#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include <sys/select.h>

#define SAMPLE_RATE 44100
#define PI 3.14159265358979323846

#define NOTE_BB3 233.08
#define NOTE_B3 246.94
#define NOTE_C4 261.63
#define NOTE_D4 293.66
#define NOTE_EB4 311.13
#define NOTE_E4 329.63
#define NOTE_F4 349.23
#define NOTE_G4 392.00
#define NOTE_GS4 415.30
#define NOTE_A4 440.00
#define NOTE_A3 220.00
#define NOTE_BB4 466.16
#define NOTE_B4 493.88
#define NOTE_C5 523.25
#define NOTE_D5 587.33
#define NOTE_EB5 622.25
#define NOTE_E5 659.25
#define NOTE_G5 783.99
#define NOTE_F5 698.46
#define NOTE_AB5 830.61
#define NOTE_BB5 932.33
#define NOTE_C6 1046.50
#define NOTE_D6 1174.66
#define NOTE_EB6 1244.51

int stop_playback = 0;
static FILE *g_audio_pipe = NULL;

/* --- AUDIO PIPE MANAGEMENT --- */

static FILE *get_audio_pipe(void)
{
    if (g_audio_pipe == NULL)
    {
        g_audio_pipe = popen("aplay -f S16_LE -r 44100 -c 1 -q", "w");
    }
    return g_audio_pipe;
}

static void close_audio_pipe(void)
{
    if (g_audio_pipe != NULL)
    {
        pclose(g_audio_pipe);
        g_audio_pipe = NULL;
    }
}

/* --- INPUT HANDLING --- */

int is_enter_pressed()
{
    struct timeval tv = {0, 0};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0)
    {
        getchar();
        return 1;
    }
    return 0;
}

/* --- AUDIO SYNTHESIS ENGINE --- */

void play_beep(float frequency, float duration_sec)
{
    if (stop_playback)
        return;

    if (is_enter_pressed())
    {
        stop_playback = 1;
        return;
    }

    FILE *pipe = get_audio_pipe();
    if (!pipe)
        return;

    int samples = SAMPLE_RATE * duration_sec;

    for (int i = 0; i < samples; i++)
    {
        double t = (double)i / SAMPLE_RATE;
        int16_t sample = (int16_t)(32767 * sin(2 * PI * frequency * t));
        fwrite(&sample, sizeof(sample), 1, pipe);
    }

    fflush(pipe);
}

void play_triad(float freq1, float freq2, float freq3, float duration_sec)
{
    if (stop_playback)
        return;

    if (is_enter_pressed())
    {
        stop_playback = 1;
        return;
    }

    FILE *pipe = get_audio_pipe();
    if (!pipe)
        return;

    int samples = SAMPLE_RATE * duration_sec;

    for (int i = 0; i < samples; i++)
    {
        double t = (double)i / SAMPLE_RATE;

        double wave1 = sin(2 * PI * freq1 * t);
        double wave2 = sin(2 * PI * freq2 * t);
        double wave3 = sin(2 * PI * freq3 * t);

        double mixed_wave = (wave1 + wave2 + wave3) / 3.0;
        int16_t sample = (int16_t)(32767 * mixed_wave);
        fwrite(&sample, sizeof(sample), 1, pipe);
    }

    fflush(pipe);
}

void play_rest(float duration_sec)
{
    if (stop_playback)
        return;

    if (is_enter_pressed())
    {
        stop_playback = 1;
        return;
    }

    usleep((int)(duration_sec * 1000000));
}

/* --- SONG COMPOSITIONS --- */

void play_some_beep()
{
    float E = 329.63;
    float D = 293.66;
    float C = 261.63;

    play_beep(E, 0.4);
    play_beep(D, 0.4);
    play_beep(C, 0.4);
    play_beep(D, 0.4);

    play_beep(E, 0.4);
    play_beep(E, 0.4);
    play_beep(E, 0.8);

    close_audio_pipe();
}
void play_mario_theme()
{
    play_beep(NOTE_E5, 0.15);
    play_rest(0.05);
    play_beep(NOTE_E5, 0.15);
    play_rest(0.15);
    play_beep(NOTE_E5, 0.15);
    play_rest(0.15);
    play_beep(NOTE_C5, 0.15);
    play_rest(0.05);
    play_beep(NOTE_E5, 0.15);
    play_rest(0.15);
    play_beep(NOTE_G5, 0.20);
    play_rest(0.40);
    play_beep(NOTE_G4, 0.20);
    play_rest(0.40);

    close_audio_pipe();
}
void play_imperial_march()
{
    play_beep(NOTE_G4, 0.40);
    play_rest(0.05);
    play_beep(NOTE_G4, 0.40);
    play_rest(0.05);
    play_beep(NOTE_G4, 0.40);
    play_rest(0.05);

    play_beep(NOTE_EB4, 0.25);
    play_beep(NOTE_BB4, 0.15);
    play_beep(NOTE_G4, 0.40);
    play_rest(0.05);

    play_beep(NOTE_EB4, 0.25);
    play_beep(NOTE_BB4, 0.15);
    play_beep(NOTE_G4, 0.80);

    close_audio_pipe();
}
void play_tetris()
{
    play_beep(NOTE_E5, 0.4);
    play_beep(NOTE_B4, 0.2);
    play_beep(NOTE_C5, 0.2);
    play_beep(NOTE_D5, 0.4);
    play_beep(NOTE_C5, 0.2);
    play_beep(NOTE_B4, 0.2);

    play_beep(NOTE_A4, 0.4);
    play_beep(NOTE_A4, 0.2);
    play_beep(NOTE_C5, 0.2);
    play_beep(NOTE_E5, 0.4);
    play_beep(NOTE_D5, 0.2);
    play_beep(NOTE_C5, 0.2);

    play_beep(NOTE_B4, 0.4);
    play_beep(NOTE_B4, 0.2);
    play_beep(NOTE_C5, 0.2);
    play_beep(NOTE_D5, 0.4);
    play_beep(NOTE_E5, 0.4);

    play_beep(NOTE_C5, 0.4);
    play_beep(NOTE_A4, 0.4);
    play_beep(NOTE_A4, 0.6);

    close_audio_pipe();
}
void play_megalovania()
{
    float D4 = 293.66, C4 = 261.63, B3 = 246.94, AS3 = 233.08;
    float D5 = 587.33, A4 = 440.00, GS4 = 415.30, G4 = 392.00, F4 = 349.23;

    float root_notes[] = {D4, C4, B3, AS3};
    int i = 0;

    while (1)
    {
        if (stop_playback)
            break;

        float root = root_notes[i];

        play_triad(root, root * 1.5, root * 2.0, 0.12);
        play_rest(0.02);
        play_triad(root, root * 1.5, root * 2.0, 0.12);
        play_rest(0.05);

        play_triad(D5 * 0.5, D5, D5 * 2.0, 0.25);
        play_rest(0.15);

        play_triad(A4 * 0.5, A4, A4 * 2.0, 0.30);
        play_rest(0.20);

        play_triad(GS4 * 0.5, GS4, GS4 * 2.0, 0.20);
        play_rest(0.05);

        play_triad(G4 * 0.5, G4, G4 * 2.0, 0.20);
        play_rest(0.05);

        play_triad(F4 * 0.5, F4, F4 * 2.0, 0.25);
        play_rest(0.05);

        play_triad(D4 * 0.5, D4, D4 * 2.0, 0.12);
        play_rest(0.02);

        play_triad(F4 * 0.5, F4, F4 * 2.0, 0.12);
        play_rest(0.02);

        play_triad(G4 * 0.5, G4, G4 * 2.0, 0.12);

        play_rest(0.40);

        i++;

        if (i >= 4)
        {
            i = 0;
        }
    }

    close_audio_pipe();
}
void play_megalovania_remix()
{
    float D4 = NOTE_D4, C4 = NOTE_C4, B3 = NOTE_B3, AS3 = NOTE_BB3;
    float D5 = NOTE_D5, A4 = NOTE_A4, GS4 = NOTE_GS4, G4 = NOTE_G4, F4 = NOTE_F4;
    float root_notes[] = {D4, C4, B3, AS3};

    float E4 = NOTE_E4, EB4 = NOTE_EB4, BB4 = NOTE_BB4;
    float D6 = NOTE_D6, C5 = NOTE_C5;

    int i = 0;
    int loop_count = 0;
    float speed = 1.0f;

    while (1)
    {
        if (stop_playback)
            break;

        float root = root_notes[i];

        speed = (loop_count % 8 < 4) ? 1.0f : 0.6f;

        play_triad(root, root * 1.5, root * 2.0, 0.12 * speed);
        play_rest(0.02 * speed);
        play_triad(root, root * 1.5, root * 2.0, 0.12 * speed);
        play_rest(0.05 * speed);

        if (loop_count % 4 == 3)
        {
            play_triad(C4 * 0.5, C4, C4 * 2.0, 0.08);
            play_rest(0.01);
            play_triad(EB4 * 0.5, EB4, EB4 * 2.0, 0.08);
            play_rest(0.01);
            play_triad(E4 * 0.5, E4, E4 * 2.0, 0.08);
            play_rest(0.02);
        }

        play_triad(D5 * 0.5, D5, D5 * 2.0, 0.25 * speed);
        play_rest(0.15 * speed);
        play_triad(A4 * 0.5, A4, A4 * 2.0, 0.30 * speed);
        play_rest(0.20 * speed);
        play_triad(GS4 * 0.5, GS4, GS4 * 2.0, 0.20 * speed);
        play_rest(0.05 * speed);
        play_triad(G4 * 0.5, G4, G4 * 2.0, 0.20 * speed);
        play_rest(0.05 * speed);
        play_triad(F4 * 0.5, F4, F4 * 2.0, 0.25 * speed);
        play_rest(0.05 * speed);
        play_triad(D4 * 0.5, D4, D4 * 2.0, 0.12 * speed);
        play_rest(0.02 * speed);
        play_triad(F4 * 0.5, F4, F4 * 2.0, 0.12 * speed);
        play_rest(0.02 * speed);
        play_triad(G4 * 0.5, G4, G4 * 2.0, 0.12 * speed);

        if (loop_count % 8 == 7)
        {
            play_triad(BB4 * 0.5, BB4, BB4 * 2.0, 0.15);
            play_rest(0.05);
            play_triad(C5 * 0.5, C5, C5 * 2.0, 0.15);
            play_rest(0.05);
            play_triad(D6 * 0.25, D6 * 0.5, D6, 0.30);
            play_rest(0.20);
        }
        else
        {
            play_rest(0.40 * speed);
        }

        i++;
        if (i >= 4)
            i = 0;
        loop_count++;
    }
    close_audio_pipe();
}
void play_megalovania_jazz_remix()
{
    float D3 = NOTE_D4 / 2.0;
    float C3 = NOTE_C4 / 2.0;
    float B2 = NOTE_B3 / 2.0;
    float AS2 = NOTE_BB3 / 2.0;

    float D5 = NOTE_D5, A4 = NOTE_A4, GS4 = NOTE_GS4;
    float G4 = NOTE_G4, F4 = NOTE_F4, D4 = NOTE_D4;

    float root_notes[] = {D3, C3, B2, AS2};
    int i = 0;

    while (1)
    {
        if (stop_playback)
            break;

        float root = root_notes[i];

        // 1. Swing Rhythm
        play_triad(root, root * 1.189, root * 1.5, 0.16);
        play_rest(0.04);
        play_triad(root, root * 1.189, root * 1.5, 0.08);
        play_rest(0.10);

        // 2. Echo effect timing
        play_triad(D5 * 0.5, D5, D5 * 1.5, 0.20);
        play_rest(0.18);

        // 3. Harmonized Melody
        play_triad(A4, NOTE_F4, A4 * 2.0, 0.25);
        play_rest(0.15);

        play_triad(GS4, NOTE_E4, GS4 * 2.0, 0.15);
        play_rest(0.08);

        play_triad(G4, NOTE_EB4, G4 * 2.0, 0.15);
        play_rest(0.08);

        play_triad(F4, F4 * 1.25, F4 * 1.5, 0.18);
        play_rest(0.06);

        // 4. Fast Arpeggio
        play_triad(D4, F4, A4, 0.10);
        play_rest(0.02);
        play_triad(F4, A4, NOTE_C5, 0.10);
        play_rest(0.02);
        play_triad(G4, NOTE_B4, NOTE_D5, 0.18);

        play_rest(0.35);

        i++;

        if (i >= 4)
        {
            i = 0;
        }
    }

    close_audio_pipe();
}
void play_megalovania_call_and_response()
{
    float D3 = NOTE_D4 / 2.0;
    float C3 = NOTE_C4 / 2.0;
    float B2 = NOTE_B3 / 2.0;
    float AS2 = NOTE_BB3 / 2.0;

    float root_notes[] = {D3, C3, B2, AS2};

    int i = 0;

    while (1)
    {
        if (stop_playback)
            break;

        float root = root_notes[i % 4];

        play_triad(root, root * 1.189, root * 1.5, 0.16);
        play_rest(0.04);
        play_triad(root, root * 1.189, root * 1.5, 0.08);
        play_rest(0.10);

        if (i < 4)
        {
            float D5 = NOTE_D5, A4 = NOTE_A4, GS4 = NOTE_GS4;
            float G4 = NOTE_G4, F4 = NOTE_F4, D4 = NOTE_D4;

            play_triad(D5 * 0.5, D5, D5 * 1.5, 0.20);
            play_rest(0.18);

            play_triad(A4, NOTE_F4, A4 * 2.0, 0.25);
            play_rest(0.15);

            play_triad(GS4, NOTE_E4, GS4 * 2.0, 0.15);
            play_rest(0.08);

            play_triad(G4, NOTE_EB4, G4 * 2.0, 0.15);
            play_rest(0.08);

            play_triad(F4, F4 * 1.25, F4 * 1.5, 0.18);
            play_rest(0.06);

            play_triad(D4, F4, A4, 0.10);
            play_rest(0.02);
            play_triad(F4, A4, NOTE_C5, 0.10);
            play_rest(0.02);
            play_triad(G4, NOTE_B4, NOTE_D5, 0.18);

            play_rest(0.35);
        }
        else
        {
            play_triad(NOTE_F4, NOTE_A4, NOTE_C5, 0.15);
            play_rest(0.05);

            play_triad(NOTE_G4, NOTE_B4, NOTE_D5, 0.15);
            play_rest(0.05);

            play_triad(NOTE_A4, NOTE_C5, NOTE_E5, 0.15);
            play_rest(0.05);

            play_triad(NOTE_C5, NOTE_E5, NOTE_G5, 0.15);
            play_rest(0.15);

            play_triad(NOTE_D5, NOTE_F5, NOTE_A4 * 2.0, 0.25);
            play_rest(0.15);

            play_triad(NOTE_D4, NOTE_F4, NOTE_A4, 0.20);
            play_rest(0.40);
        }

        i++;

        if (i >= 8)
        {
            i = 0;
        }
    }

    close_audio_pipe();
}
void play_megalovania_pvz_style()
{
    float D2 = NOTE_D4 / 4.0;
    float C2 = NOTE_C4 / 4.0;
    float B1 = NOTE_B3 / 4.0;
    float AS1 = NOTE_BB3 / 4.0;

    float root_notes[] = {D2, C2, B1, AS1};
    int i = 0;

    while (1)
    {
        if (stop_playback)
            break;

        float root = root_notes[i];

        // --- Oom-Pah Rhythm ---
        play_triad(root, root * 2.0, root * 2.0, 0.08);
        play_rest(0.08);
        play_triad(root, root * 2.0, root * 2.0, 0.08);
        play_rest(0.12);

        // --- Glockenspiel Melody ---
        float D5 = NOTE_D5, A4 = NOTE_A4, GS4 = NOTE_GS4;
        float G4 = NOTE_G4, F4 = NOTE_F4, D4 = NOTE_D4;

        play_triad(D5, D5 * 2.0, D5 * 4.0, 0.15);
        play_rest(0.20);

        play_triad(root * 3.0, root * 4.0, root * 5.0, 0.06);
        play_rest(0.06);

        play_triad(A4, A4 * 2.0, A4 * 4.0, 0.15);
        play_rest(0.15);

        play_triad(GS4, GS4 * 2.0, GS4 * 4.0, 0.08);
        play_rest(0.08);

        play_triad(G4, G4 * 2.0, G4 * 4.0, 0.08);
        play_rest(0.08);

        play_triad(F4, F4 * 2.0, F4 * 4.0, 0.12);
        play_rest(0.10);

        play_triad(D4, D4 * 2.0, D4 * 4.0, 0.08);
        play_rest(0.05);
        play_triad(F4, F4 * 2.0, F4 * 4.0, 0.08);
        play_rest(0.05);

        // --- Zombie Groan ---
        if (i % 2 == 1)
        {
            play_triad(G4, G4 * 2.0, G4 * 2.119, 0.20);
        }
        else
        {
            play_triad(G4, G4 * 2.0, G4 * 4.0, 0.15);
        }

        play_rest(0.35);

        i++;

        if (i >= 4)
        {
            i = 0;
        }
    }

    close_audio_pipe();
}
void play_hard_to_say_goodbye()
{
    play_beep(NOTE_G5, 0.5);
    play_beep(NOTE_C6, 0.5);
    play_beep(NOTE_EB6, 1.0);

    play_beep(NOTE_D6, 0.4);
    play_beep(NOTE_C6, 0.4);
    play_beep(NOTE_G5, 1.2);

    play_rest(0.3);

    play_beep(NOTE_F5, 0.5);
    play_beep(NOTE_AB5, 0.5);
    play_beep(NOTE_C6, 1.0);

    play_beep(NOTE_BB5, 0.4);
    play_beep(NOTE_AB5, 0.4);
    play_beep(NOTE_G5, 1.2);

    play_rest(0.3);

    play_beep(NOTE_EB5, 0.5);
    play_beep(NOTE_G5, 0.5);
    play_beep(NOTE_C6, 1.0);

    play_beep(NOTE_D6, 0.8);
    play_beep(NOTE_C6, 1.5);

    close_audio_pipe();
}
void play_finish_music()
{
    stop_playback = 0;
    float C3 = 130.81, F3 = 174.61, G3 = 196.00, A3 = 220.00;
    float B3 = 246.94, C4 = 261.63, D4 = 293.66, E4 = 329.63;
    float G4 = 392.00;

    for (int i = 0; i < 2; i++)
    {
        if (stop_playback)
            break;

        play_triad(A3, C4, E4, 0.8);
        play_triad(F3, A3, C4, 0.8);
        play_triad(C4, E4, G4, 0.8);
        play_triad(G3, B3, D4, 0.8);
    }

    play_triad(A3, C4, E4, 0.4);
    play_triad(A3, C4, E4, 0.4);

    play_triad(F3, A3, C4, 0.4);
    play_triad(F3, A3, C4, 0.4);

    play_triad(C4, E4, G4, 0.4);
    play_triad(C4, E4, G4, 0.4);

    play_triad(G3, B3, D4, 0.4);
    play_triad(G3, B3, D4, 0.4);

    play_triad(C3, E4, G4, 2.5);

    close_audio_pipe();
}
void play_cinematic_ambient()
{
    float F2 = 87.31, C3 = 130.81, A3 = 220.00;
    float C2 = 65.41, G2 = 98.00, E4 = 329.63;
    float E2 = 82.41, B2 = 123.47, G3 = 196.00;
    float A2 = 110.00, E3 = 164.81, C4 = 261.63;

    float B3 = 246.94, D4 = 293.66;

    play_triad(F2, C3, A3, 2.5);
    play_triad(C2, G2, E4, 2.5);
    play_triad(E2, B2, G3, 2.5);
    play_triad(A2, E3, C4, 2.5);

    play_triad(F2, C3, G3, 2.5);
    play_triad(C2, G2, C4, 2.5);
    play_triad(E2, B2, D4, 2.5);

    play_triad(A2, C3, B3, 4.0);

    close_audio_pipe();
}
void play_garden_guardian(void)
{
    int repeat;
    stop_playback = 0;

    for (repeat = 0; repeat < 2 && !stop_playback; repeat++)
    {
        play_beep(NOTE_A3, 0.12);
        play_rest(0.04);
        play_beep(NOTE_C4, 0.12);
        play_rest(0.04);
        play_beep(NOTE_EB4, 0.12);
        play_rest(0.04);
        play_beep(NOTE_E4, 0.18);
        play_rest(0.10);

        play_beep(NOTE_F4, 0.12);
        play_rest(0.04);
        play_beep(NOTE_GS4, 0.12);
        play_rest(0.04);
        play_beep(NOTE_G4, 0.12);
        play_rest(0.04);
        play_beep(NOTE_F4, 0.18);
        play_rest(0.10);

        play_beep(NOTE_E4, 0.10);
        play_beep(NOTE_D4, 0.10);
        play_beep(NOTE_C4, 0.10);
        play_beep(NOTE_A3, 0.20);
        play_rest(0.15);

        play_beep(NOTE_D4, 0.10);
        play_beep(NOTE_EB4, 0.10);
        play_beep(NOTE_F4, 0.10);
        play_beep(NOTE_A4, 0.25);
        play_rest(0.20);

        play_triad(NOTE_A3, NOTE_C4, NOTE_EB4, 0.35);
        play_rest(0.15);
    }

    play_triad(NOTE_A3, NOTE_C5, NOTE_EB5, 1.2);

    close_audio_pipe();
}
void play_final_showdown(void)
{
    float A3 = 220.00, F3 = 174.61, G3 = 196.00;
    int r, i;

    stop_playback = 0;

    play_triad(A3, NOTE_C4, NOTE_E4, 0.6);
    play_rest(0.05);
    play_triad(F3, A3, NOTE_C4, 0.6);
    play_rest(0.05);
    play_triad(NOTE_C4, NOTE_E4, NOTE_G4, 0.6);
    play_rest(0.05);
    play_triad(G3, NOTE_B3, NOTE_D4, 0.6);
    play_rest(0.2);

    for (r = 0; r < 2 && !stop_playback; r++)
    {
        play_triad(A3, NOTE_C4, NOTE_E4, 0.18);
        play_triad(A3, NOTE_C4, NOTE_E4, 0.18);
        play_triad(F3, A3, NOTE_C4, 0.18);
        play_triad(F3, A3, NOTE_C4, 0.18);
        play_triad(NOTE_C4, NOTE_E4, NOTE_G4, 0.18);
        play_triad(NOTE_C4, NOTE_E4, NOTE_G4, 0.18);
        play_triad(G3, NOTE_B3, NOTE_D4, 0.18);
        play_triad(G3, NOTE_B3, NOTE_D4, 0.18);
    }

    play_triad(NOTE_E4, NOTE_GS4, NOTE_B4, 0.8);
    play_rest(0.15);

    for (i = 0; i < 6 && !stop_playback; i++)
    {
        play_triad(A3, A3 * 1.5f, A3 * 2.0f, 0.10);
        play_triad(F3, F3 * 1.5f, F3 * 2.0f, 0.10);
    }

    play_triad(A3 / 2.0f, NOTE_E4, NOTE_A4, 2.5);

    close_audio_pipe();
}
void play_victory_fanfare(void)
{
    float C3 = 130.81;
    int r, i;

    stop_playback = 0;

    play_beep(NOTE_C4, 0.08);
    play_beep(NOTE_E4, 0.08);
    play_beep(NOTE_G4, 0.08);
    play_beep(NOTE_C5, 0.15);
    play_rest(0.05);
    play_triad(NOTE_C4, NOTE_E4, NOTE_G4, 0.5);
    play_rest(0.15);

    play_triad(NOTE_C4, NOTE_E4, NOTE_G4, 0.4);
    play_rest(0.03);
    play_triad(NOTE_G4, NOTE_B4, NOTE_D5, 0.4);
    play_rest(0.03);
    play_triad(NOTE_A4, NOTE_C5, NOTE_E5, 0.4);
    play_rest(0.03);
    play_triad(NOTE_F4, NOTE_A4, NOTE_C5, 0.4);
    play_rest(0.03);
    play_triad(NOTE_C4, NOTE_E4, NOTE_G4, 0.6);
    play_rest(0.2);

    for (r = 0; r < 2 && !stop_playback; r++)
    {
        play_triad(NOTE_C4, NOTE_E4, NOTE_G4, 0.14);
        play_rest(0.04);
        play_triad(NOTE_G4, NOTE_B4, NOTE_D5, 0.14);
        play_rest(0.04);
        play_triad(NOTE_A4, NOTE_C5, NOTE_E5, 0.14);
        play_rest(0.04);
        play_triad(NOTE_F4, NOTE_A4, NOTE_C5, 0.14);
        play_rest(0.04);
    }

    {
        float scale[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};

        for (i = 0; i < 8 && !stop_playback; i++)
            play_beep(scale[i], 0.08);
    }
    play_rest(0.1);

    play_triad(C3, NOTE_G4, NOTE_C5, 2.5);

    close_audio_pipe();
}
void play_addictive_melody()
{
    float hook[8] = {NOTE_C5, NOTE_C5, NOTE_EB5, NOTE_C5, NOTE_G4, NOTE_BB4, NOTE_C5, NOTE_D5};
    float dur[8] = {0.10, 0.10, 0.15, 0.10, 0.10, 0.15, 0.10, 0.25};
    float rest[8] = {0.02, 0.02, 0.05, 0.02, 0.02, 0.05, 0.02, 0.15};
    float bass[4] = {NOTE_C4, NOTE_A3, NOTE_F4, NOTE_G4};
    int i = 0;
    int j = 0;
    int loop = 0;
    while (1)
    {
        if (stop_playback)
            break;
        play_triad(bass[j], hook[i], hook[i] * 1.5, dur[i]);
        play_rest(rest[i]);
        i++;
        if (i >= 8)
        {
            i = 0;
            j++;
            if (j >= 4)
                j = 0;
            loop++;
            if (loop % 4 == 0)
            {
                play_triad(NOTE_C5, NOTE_EB5, NOTE_G5, 0.05);
                play_rest(0.02);
                play_triad(NOTE_D5, NOTE_F5, NOTE_A4, 0.05);
                play_rest(0.02);
                play_triad(NOTE_EB5, NOTE_G5, NOTE_BB4, 0.05);
                play_rest(0.02);
                play_triad(NOTE_C6, NOTE_C6, NOTE_C6, 0.30);
                play_rest(0.20);
            }
        }
    }
    close_audio_pipe();
}

/* --- MAIN PROGRAM - TESTER --- */

/*int main()
{
    while (1)
    {
        play_megalovania_pvz_style();
    }
    return 0;
}*/