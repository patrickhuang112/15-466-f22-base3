#include "Game.hpp"

namespace Game {

void Game::display_correct_letter(){

}

void Game::display_wrong_letter() {

}

void Game::begin_playing_word_audio() {
    capture_input = false; 
    current_audio_letter = 0;
}

void Game::play_word_audio() {
    assert(!capture_input);
    uint32_t len = static_cast<uint32_t>(WORD_LIST[current_word].length());
    assert(current_audio_letter >= 0 && current_audio_letter < len);

    if (current != nullptr) {
        if (current->stopped) {
            ++current_audio_letter; 
            current = nullptr;
        }
    }
    else {
        Sound::Sample& s = audio.find(WORD_LIST[current_word][current_audio_letter])->second;
        current = Sound::play(s); 
    }
    if (current_audio_letter == len) {
        begin_word_capture(); 
    }
}

void Game::begin_word_capture() {
    uint32_t len = static_cast<uint32_t>(WORD_LIST[current_word].length());
    if (hard) {
        // randomized order TODO
        for (uint32_t i = 0; i < len; ++i) {
            match_order.push_back(len - i - 1);
        }
    }
    // Easy mode, do reverse ordering
    else {
        for (uint32_t i = 0; i < len; ++i) {
            match_order.push_back(len - i - 1);
        }
    }
    capture_input = true;
}

bool Game::match_letter(char c) {
    if (c == WORD_LIST[current_word][match_order[current_word_matched]]) {
        return true;
    }
    return false;
}

void Game::next_letter() {
    ++current_word_matched;
}

bool Game::word_matched() {
    return current_word_matched == WORD_LIST[current_word].length() ? true : false; 
}

void Game::next_word() {
    ++current_word;
    current_word_matched = 0;
    match_order.clear();
    
}


}