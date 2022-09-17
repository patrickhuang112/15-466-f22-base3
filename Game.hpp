#pragma once

#include "Scene.hpp"
#include "Sound.hpp"

#include <stdint.h>
#include <vector>
#include <array>
#include <unordered_map>
#include <memory>
#include <cassert>

namespace Game {

constexpr uint32_t WORD_LIST_SIZE = 10;

constexpr uint32_t NUM_SOUNDS = 36;
constexpr std::array<std::pair<char, const char *>, NUM_SOUNDS> SOUND_PATHS = {
    std::pair('1', "1.wav"),
    std::pair('2', "2.wav"),
    std::pair('3', "3.wav"),
    std::pair('4', "4.wav"),
    std::pair('5', "5.wav"),
    std::pair('6', "6.wav"),
    std::pair('7', "7.wav"),
    std::pair('8', "8.wav"),
    std::pair('9', "8.wav"),
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
const std::array<std::string, WORD_LIST_SIZE> WORD_LIST {
    "1234567890987234102831",
    "1234",
    "ok" 
};


struct Letter {
    char letter; 
};

struct Word {
    Word (const char *w) {
        word = std::string(w);
    }
    private:
        std::string word;
};


struct Game {
 

    Game() {
        current_word = 0;
        current_word_matched = 0;
        current_audio_letter = 0;
        current = nullptr;
        hard = false;
        capture_input = false;
        // Load all audio samples
        for (const auto& p : SOUND_PATHS) {
            assert(audio.find(p.first) == audio.end());
            audio.emplace(p.first, Sound::Sample(std::string(p.second)));
        } 
    } 

    ~Game() = default;

    bool capture_input;
    void begin_playing_word_audio();
    void play_word_audio();
    void begin_word_capture();
    void display_correct_letter();
    void display_wrong_letter();
    void next_letter();
    bool match_letter(char c);
    bool word_matched();
    void next_word();
    

    private:
        std::unordered_map<char, Sound::Sample> audio;
        std::vector<uint32_t> match_order;
        uint32_t current_audio_letter;
        uint32_t current_word_matched;
        uint32_t current_word; 
        std::shared_ptr<Sound::PlayingSample> current; 
        bool hard;

};

}
