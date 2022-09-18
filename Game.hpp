#pragma once

#include "Scene.hpp"
#include "Sound.hpp"
#include "data_path.hpp"

#include <stdint.h>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace Game {

constexpr uint32_t WORD_LIST_SIZE = 5;
constexpr float INCORRECT_FADE = 3.f;
constexpr uint32_t NUM_SOUNDS = 36;
const glm::u8vec4 INCORRECT_COLOR = glm::u8vec4(0xf4, 0x04, 0x2c, 0x00);
const glm::u8vec4 CORRECT_COLOR = glm::u8vec4(0x44, 0xed, 0x69, 0x00);
const glm::u8vec4 DEFAULT_COLOR = glm::u8vec4(0xff, 0xff, 0xff, 0x00);
const glm::u8vec4 SELECTED_COLOR = glm::u8vec4(0x9f, 0x00, 0xcc, 0x00); 
constexpr std::array<std::pair<char, const char *>, NUM_SOUNDS> SOUND_PATHS = {
    std::pair('1', "1.wav"),
    std::pair('2', "2.wav"),
    std::pair('3', "3.wav"),
    std::pair('4', "4.wav"),
    std::pair('5', "5.wav"),
    std::pair('6', "6.wav"),
    std::pair('7', "7.wav"),
    std::pair('8', "8.wav"),
    std::pair('9', "9.wav"),
    std::pair('0', "0.wav"),
    std::pair('a', "a.wav"),
    std::pair('b', "b.wav"),
    std::pair('c', "c.wav"),
    std::pair('d', "d.wav"),
    std::pair('e', "e.wav"),
    std::pair('f', "f.wav"),
    std::pair('g', "g.wav"),
    std::pair('h', "h.wav"),
    std::pair('i', "i.wav"),
    std::pair('j', "j.wav"),
    std::pair('k', "k.wav"),
    std::pair('l', "l.wav"),
    std::pair('m', "m.wav"),
    std::pair('n', "n.wav"),
    std::pair('o', "o.wav"),
    std::pair('p', "p.wav"),
    std::pair('q', "q.wav"),
    std::pair('r', "r.wav"),
    std::pair('s', "s.wav"),
    std::pair('t', "t.wav"),
    std::pair('u', "u.wav"),
    std::pair('v', "v.wav"),
    std::pair('w', "w.wav"),
    std::pair('x', "x.wav"),
    std::pair('y', "y.wav"),
    std::pair('z', "z.wav"),
};
constexpr std::array<std::pair<char, const char *>, NUM_SOUNDS> HARD_SOUND_PATHS = {
    std::pair('1', "12.wav"),
    std::pair('2', "22.wav"),
    std::pair('3', "32.wav"),
    std::pair('4', "42.wav"),
    std::pair('5', "52.wav"),
    std::pair('6', "62.wav"),
    std::pair('7', "72.wav"),
    std::pair('8', "82.wav"),
    std::pair('9', "92.wav"),
    std::pair('0', "02.wav"),
    std::pair('a', "a2.wav"),
    std::pair('b', "b2.wav"),
    std::pair('c', "c2.wav"),
    std::pair('d', "d2.wav"),
    std::pair('e', "e2.wav"),
    std::pair('f', "f2.wav"),
    std::pair('g', "g2.wav"),
    std::pair('h', "h2.wav"),
    std::pair('i', "i2.wav"),
    std::pair('j', "j2.wav"),
    std::pair('k', "k2.wav"),
    std::pair('l', "l2.wav"),
    std::pair('m', "m2.wav"),
    std::pair('n', "n2.wav"),
    std::pair('o', "o2.wav"),
    std::pair('p', "p2.wav"),
    std::pair('q', "q2.wav"),
    std::pair('r', "r2.wav"),
    std::pair('s', "s2.wav"),
    std::pair('t', "t2.wav"),
    std::pair('u', "u2.wav"),
    std::pair('v', "v2.wav"),
    std::pair('w', "w2.wav"),
    std::pair('x', "x2.wav"),
    std::pair('y', "y2.wav"),
    std::pair('z', "z2.wav"),
};

const std::string INTRO_AUDIO_PATH = "intro.wav";
const std::string TRANSITION_AUDIO_PATH = "transition.wav";
// If you want custom words, please don't put more than 21 character sequences as
// it won't all be drawn on the screen
// ALSO PLEASE DON'T USE SPACES
const std::array<std::string, WORD_LIST_SIZE> WORD_LIST {
    "easierone",
    "slightlyharder",
    "thiisoneishaard",
    "thiisnoooowahrdder",
    "aofjekbnaksofiejalbp"
};


struct Letter {
    Letter(char c) {
        letter = c;
        displayed = false; 
        incorrect = false;
        incorrect_time = 0.f;
    }
    char letter; 
    bool displayed;
    bool incorrect;
    float incorrect_time; 
};

struct Word {
    Word (const char *w) {
        word = std::string(w);
    }
    private:
        std::string word;
};

enum AudioState {
    Transition,
    Word,
    Intro,
    Capture
};


struct Game {
 

    Game() : intro_audio(data_path(INTRO_AUDIO_PATH)), 
             transition_audio(data_path(TRANSITION_AUDIO_PATH)) {
        current_word = 0;
        current_word_matched = 0;
        current_audio_letter = 0;
        current = nullptr;
        hard = true;
        capture_input = true;
        game_over = false;
        difficulty_selected = false;
        state = Intro;
        // Load all audio samples
        for (const auto& p : SOUND_PATHS) {
            assert(audio.find(p.first) == audio.end());
            audio.emplace(p.first, Sound::Sample(data_path(std::string(p.second))));
        } 
        // Load all hard mode audio samples
        for (const auto& p : HARD_SOUND_PATHS) {
            assert(hard_audio.find(p.first) == hard_audio.end());
            hard_audio.emplace(p.first, Sound::Sample(data_path(std::string(p.second))));
        } 
        time_passed = 0.f;
        score = 0.f;
        state = Intro;
    } 

    ~Game() = default;

    bool capture_input;
    bool game_over;
    bool difficulty_selected;
    float time_passed;
    float score;
    bool hard;
    AudioState state;
    uint32_t current_selected();
    void begin_playing_word_audio();
    void play_intro_audio();
    bool play_transition_audio();
    void play_word_audio(float elapsed);
    void begin_word_capture();
    void mark_incorrect();
    void mark_correct();
    void next_letter();
    bool match_letter(char c);
    bool word_matched();
    bool next_word();
    std::shared_ptr<Sound::PlayingSample> current; 
    std::vector<Letter> letters;
    

    private:
        Sound::Sample intro_audio; 
        Sound::Sample transition_audio;
        std::unordered_map<char, Sound::Sample> audio;
        std::unordered_map<char, Sound::Sample> hard_audio;
        std::vector<uint32_t> match_order;
        uint32_t current_audio_letter;
        uint32_t current_word_matched;
        uint32_t current_word; 
};

}
