#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include <sys/select.h>

#define SAMPLE_RATE 44100
#define PI 3.14159265358979323846

// --- THE ULTIMATE NOTE DICTIONARY ---
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
int is_enter_pressed()
{
    struct timeval tv = {0, 0}; // 0 timeout means "do not wait"
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
void play_beep(float frequency, float duration_sec)
{
    // 1. Add these checks so single notes can be interrupted!
    if (stop_playback)
        return;

    if (is_enter_pressed())
    {
        stop_playback = 1;
        return;
    }

    FILE *pipe = popen("aplay -f S16_LE -r 44100 -c 1 -q", "w");
    if (!pipe)
        return;

    int samples = SAMPLE_RATE * duration_sec;

    for (int i = 0; i < samples; i++)
    {
        double t = (double)i / SAMPLE_RATE;
        // Sine wave generation
        int16_t sample = (int16_t)(32767 * sin(2 * PI * frequency * t));
        fwrite(&sample, sizeof(sample), 1, pipe);
    }

    pclose(pipe);
    usleep(20000);
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

    FILE *pipe = popen("aplay -f S16_LE -r 44100 -c 1 -q", "w");
    if (!pipe)
        return;

    int samples = SAMPLE_RATE * duration_sec;

    for (int i = 0; i < samples; i++)
    {
        double t = (double)i / SAMPLE_RATE;

        // Generate the math for all three notes
        double wave1 = sin(2 * PI * freq1 * t);
        double wave2 = sin(2 * PI * freq2 * t);
        double wave3 = sin(2 * PI * freq3 * t);

        // Mix them together and divide by 3 to prevent clipping/distortion
        double mixed_wave = (wave1 + wave2 + wave3) / 3.0;

        // Convert the mixed math into 16-bit audio
        int16_t sample = (int16_t)(32767 * mixed_wave);
        fwrite(&sample, sizeof(sample), 1, pipe);
    }

    pclose(pipe);
    usleep(20000);
}
void play_rest(float duration_sec)
{
    // If the flag is tripped, do nothing and return immediately
    if (stop_playback)
        return;

    // Check if the user pressed enter right now
    if (is_enter_pressed())
    {
        stop_playback = 1; // Trip the flag
        return;            // Abort the rest
    }

    usleep((int)(duration_sec * 1000000));
}
/*------------------------------------------------------------*/
void play_some_beep()
{
    // We no longer need to open the pipe here, because play_beep()
    // now does it automatically for every note!

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
}
void play_mario_theme()
{
    // The classic intro
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
}
void play_imperial_march()
{

    // Bum.. Bum.. Bum.. bum-ba-dum, bum-ba-dum
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
}
void play_megalovania()
{
    // Define the precise float frequencies for the song
    float D4 = 293.66, C4 = 261.63, B3 = 246.94, AS3 = 233.08;
    float D5 = 587.33, A4 = 440.00, GS4 = 415.30, G4 = 392.00, F4 = 349.23;

    // The 4 starting bass notes for each phrase of the intro
    float root_notes[] = {D4, C4, B3, AS3};

    int i = 0; // Start our counter at 0

    // INFINITE LOOP: Keep playing until the user hits Enter!
    while (1)
    {
        // Instantly break the infinite loop if the alarm was stopped
        if (stop_playback)
            break;

        float root = root_notes[i];

        // 1. BASS NOTES
        play_triad(root, root * 1.5, root * 2.0, 0.12);
        play_rest(0.02);
        play_triad(root, root * 1.5, root * 2.0, 0.12);
        play_rest(0.05);

        // 2. MELODY NOTES
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

        // Back to the lower octave for the fast tail-end lick
        play_triad(D4 * 0.5, D4, D4 * 2.0, 0.12);
        play_rest(0.02);

        play_triad(F4 * 0.5, F4, F4 * 2.0, 0.12);
        play_rest(0.02);

        play_triad(G4 * 0.5, G4, G4 * 2.0, 0.12);

        // 3. Pause before the sequence repeats
        play_rest(0.40);

        // Advance our counter to the next bass note
        i++;

        // If we've played all 4 bass notes, loop back to the first one!
        if (i >= 4)
        {
            i = 0;
        }
    }
}
void play_hard_to_say_goodbye()
{
    // First melancholic phrase
    play_beep(NOTE_G5, 0.5);
    play_beep(NOTE_C6, 0.5);
    play_beep(NOTE_EB6, 1.0);

    play_beep(NOTE_D6, 0.4);
    play_beep(NOTE_C6, 0.4);
    play_beep(NOTE_G5, 1.2);

    play_rest(0.3);

    // Second descending phrase
    play_beep(NOTE_F5, 0.5);
    play_beep(NOTE_AB5, 0.5);
    play_beep(NOTE_C6, 1.0);

    play_beep(NOTE_BB5, 0.4);
    play_beep(NOTE_AB5, 0.4);
    play_beep(NOTE_G5, 1.2);

    play_rest(0.3);

    // Resolution
    play_beep(NOTE_EB5, 0.5);
    play_beep(NOTE_G5, 0.5);
    play_beep(NOTE_C6, 1.0);

    play_beep(NOTE_D6, 0.8);
    play_beep(NOTE_C6, 1.5); // Final fading note
}
void play_finish_music()
{
    // Define standard musical frequencies (Equal Temperament)
    float C3 = 130.81, F3 = 174.61, G3 = 196.00, A3 = 220.00;
    float B3 = 246.94, C4 = 261.63, D4 = 293.66, E4 = 329.63;
    float G4 = 392.00;

    // Play the main progression twice, slowly
    for (int i = 0; i < 2; i++)
    {
        // Instantly break the loop if the alarm was stopped
        if (stop_playback)
            break;

        // A minor (A3, C4, E4)
        play_triad(A3, C4, E4, 0.8);

        // F major (F3, A3, C4)
        play_triad(F3, A3, C4, 0.8);

        // C major (C4, E4, G4)
        play_triad(C4, E4, G4, 0.8);

        // G major (G3, B3, D4)
        play_triad(G3, B3, D4, 0.8);
    }

    // Play the same chords, but faster (0.4s each) to build momentum
    play_triad(A3, C4, E4, 0.4);
    play_triad(A3, C4, E4, 0.4);

    play_triad(F3, A3, C4, 0.4);
    play_triad(F3, A3, C4, 0.4);

    play_triad(C4, E4, G4, 0.4);
    play_triad(C4, E4, G4, 0.4);

    play_triad(G3, B3, D4, 0.4);
    play_triad(G3, B3, D4, 0.4);

    // End on a massive, resolving C major chord that spans multiple octaves
    play_triad(C3, E4, G4, 2.5);
}
void play_cinematic_ambient()
{
    // A dreamy, wide-spaced set of chords (spanning lower bass notes and high melodies)
    float F2 = 87.31, C3 = 130.81, A3 = 220.00;  // F Major (Wide)
    float C2 = 65.41, G2 = 98.00, E4 = 329.63;   // C Major (Deep bass, high top)
    float E2 = 82.41, B2 = 123.47, G3 = 196.00;  // E minor
    float A2 = 110.00, E3 = 164.81, C4 = 261.63; // A minor

    // --> FIXED: Added the missing frequencies for B3 and D4! <--
    float B3 = 246.94, D4 = 293.66;

    // Play each chord for a long 2.5 seconds to let the fades breathe
    play_triad(F2, C3, A3, 2.5);
    play_triad(C2, G2, E4, 2.5);
    play_triad(E2, B2, G3, 2.5);
    play_triad(A2, E3, C4, 2.5);

    // Second time through: shift the top note slightly to create a gentle melody
    play_triad(F2, C3, G3, 2.5); // Turns into Fsus2
    play_triad(C2, G2, C4, 2.5);
    play_triad(E2, B2, D4, 2.5); // Turns into Em7 (D4 is now defined)

    // Final long resolving chord (A minor 9), played for 4 seconds
    play_triad(A2, C3, B3, 4.0); // B3 is now defined
}
