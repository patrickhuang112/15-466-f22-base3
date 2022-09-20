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
    std::pair('1', "sounds/1.opus"),
    std::pair('2', "sounds/2.opus"),
    std::pair('3', "sounds/3.opus"),
    std::pair('4', "sounds/4.opus"),
    std::pair('5', "sounds/5.opus"),
    std::pair('6', "sounds/6.opus"),
    std::pair('7', "sounds/7.opus"),
    std::pair('8', "sounds/8.opus"),
    std::pair('9', "sounds/9.opus"),
    std::pair('0', "sounds/0.opus"),
    std::pair('a', "sounds/a.opus"),
    std::pair('b', "sounds/b.opus"),
    std::pair('c', "sounds/c.opus"),
    std::pair('d', "sounds/d.opus"),
    std::pair('e', "sounds/e.opus"),
    std::pair('f', "sounds/f.opus"),
    std::pair('g', "sounds/g.opus"),
    std::pair('h', "sounds/h.opus"),
    std::pair('i', "sounds/i.opus"),
    std::pair('j', "sounds/j.opus"),
    std::pair('k', "sounds/k.opus"),
    std::pair('l', "sounds/l.opus"),
    std::pair('m', "sounds/m.opus"),
    std::pair('n', "sounds/n.opus"),
    std::pair('o', "sounds/o.opus"),
    std::pair('p', "sounds/p.opus"),
    std::pair('q', "sounds/q.opus"),
    std::pair('r', "sounds/r.opus"),
    std::pair('s', "sounds/s.opus"),
    std::pair('t', "sounds/t.opus"),
    std::pair('u', "sounds/u.opus"),
    std::pair('v', "sounds/v.opus"),
    std::pair('w', "sounds/w.opus"),
    std::pair('x', "sounds/x.opus"),
    std::pair('y', "sounds/y.opus"),
    std::pair('z', "sounds/z.opus")
};
constexpr std::array<std::pair<char, const char *>, NUM_SOUNDS> HARD_SOUND_PATHS = {
    std::pair('1', "sounds/12.opus"),
    std::pair('2', "sounds/22.opus"),
    std::pair('3', "sounds/32.opus"),
    std::pair('4', "sounds/42.opus"),
    std::pair('5', "sounds/52.opus"),
    std::pair('6', "sounds/62.opus"),
    std::pair('7', "sounds/72.opus"),
    std::pair('8', "sounds/82.opus"),
    std::pair('9', "sounds/92.opus"),
    std::pair('0', "sounds/02.opus"),
    std::pair('a', "sounds/a2.opus"),
    std::pair('b', "sounds/b2.opus"),
    std::pair('c', "sounds/c2.opus"),
    std::pair('d', "sounds/d2.opus"),
    std::pair('e', "sounds/e2.opus"),
    std::pair('f', "sounds/f2.opus"),
    std::pair('g', "sounds/g2.opus"),
    std::pair('h', "sounds/h2.opus"),
    std::pair('i', "sounds/i2.opus"),
    std::pair('j', "sounds/j2.opus"),
    std::pair('k', "sounds/k2.opus"),
    std::pair('l', "sounds/l2.opus"),
    std::pair('m', "sounds/m2.opus"),
    std::pair('n', "sounds/n2.opus"),
    std::pair('o', "sounds/o2.opus"),
    std::pair('p', "sounds/p2.opus"),
    std::pair('q', "sounds/q2.opus"),
    std::pair('r', "sounds/r2.opus"),
    std::pair('s', "sounds/s2.opus"),
    std::pair('t', "sounds/t2.opus"),
    std::pair('u', "sounds/u2.opus"),
    std::pair('v', "sounds/v2.opus"),
    std::pair('w', "sounds/w2.opus"),
    std::pair('x', "sounds/x2.opus"),
    std::pair('y', "sounds/y2.opus"),
    std::pair('z', "sounds/z2.opus")
};

const std::string INTRO_AUDIO_PATH = "sounds/intro.opus";
const std::string TRANSITION_AUDIO_PATH = "sounds/transition.opus";
// If you want custom words, please don't put more than 21 character sequences as
// it won't all be drawn on the screen
// ALSO PLEASE DON'T USE SPACES
const std::array<std::string, WORD_LIST_SIZE> WORD_LIST {
    "easy",
    "hhaarder",
    "thiisaneishaard",
    "thsnoooooowahrdir",
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
        replay = false;
        mistakes = 0;
        replays = 0;
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
    uint32_t mistakes;
    uint32_t replays;
    bool hard;
    bool replay;
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
