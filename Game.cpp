#include "Game.hpp"

namespace Game {

uint32_t Game::current_selected() {
    return match_order[current_word_matched];
}

void Game::mark_correct(){
    letters[match_order[current_word_matched]].incorrect = false;
    letters[match_order[current_word_matched]].displayed = true;
}

void Game::mark_incorrect() {
    letters[match_order[current_word_matched]].incorrect = true;
    letters[match_order[current_word_matched]].incorrect_time = 0.f; 
}

void Game::begin_playing_word_audio() {
    capture_input = false; 
    current_audio_letter = 0;
}

void Game::play_intro_audio() {
    current = Sound::play(intro_audio); 
}

bool Game::play_transition_audio() {
    assert(!capture_input); 
    if (current != nullptr) {
        if (current->stopped) {
            current = nullptr;
            return true;
        } 
    }
    else {
        current = Sound::play(transition_audio);
    }
    return false;
}

void Game::play_word_audio(float elapsed) {
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
        if (hard) {
            Sound::Sample& s = hard_audio.find(WORD_LIST[current_word][current_audio_letter])->second;
            current = Sound::play(s);
        }
        else {
            // For easy mode, add a slight delay between sounds
            if (time_passed < 0.5f) {
                time_passed += elapsed;
                return; 
            }
            time_passed = 0.f;
            Sound::Sample& s = audio.find(WORD_LIST[current_word][current_audio_letter])->second;
            current = Sound::play(s);
        }
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
    for (uint32_t i = 0; i < len; ++i) {
        letters.emplace_back(Letter(WORD_LIST[current_word][i])); 
    }
    capture_input = true;
    state = Capture;
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

bool Game::next_word() {
    score += time_passed;
    time_passed = 0.f;
    current_word_matched = 0;
    letters.clear();
    match_order.clear();
    ++current_word; 
    state = Transition;
    if (current_word == WORD_LIST_SIZE) {
        game_over = true;
        printf("SCORE: %f\n", score);
        return true;
    }
    return false;
}


}