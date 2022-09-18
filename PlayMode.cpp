#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

GLuint hexapod_meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > hexapod_meshes(LoadTagDefault, []() -> MeshBuffer const * {
	MeshBuffer const *ret = new MeshBuffer(data_path("hexapod.pnct"));
	hexapod_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret;
});

Load< Scene > hexapod_scene(LoadTagDefault, []() -> Scene const * {
	return new Scene(data_path("hexapod.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name){
		Mesh const &mesh = hexapod_meshes->lookup(mesh_name);

		scene.drawables.emplace_back(transform);
		Scene::Drawable &drawable = scene.drawables.back();

		drawable.pipeline = lit_color_texture_program_pipeline;

		drawable.pipeline.vao = hexapod_meshes_for_lit_color_texture_program;
		drawable.pipeline.type = mesh.type;
		drawable.pipeline.start = mesh.start;
		drawable.pipeline.count = mesh.count;

	});
});

Load< Sound::Sample > dusty_floor_sample(LoadTagDefault, []() -> Sound::Sample const * {
	return new Sound::Sample(data_path("dusty-floor.opus"));
});

PlayMode::PlayMode() : scene(*hexapod_scene) {
	//get pointer to camera for convenience:
	if (scene.cameras.size() != 1) throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();

	game.play_intro_audio();
}

PlayMode::~PlayMode() {
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size) {
	if (!game.capture_input || game.game_over) {
		return false;	
	}
	// Intro
	if (game.state == Game::Intro) {
		if (evt.type == SDL_KEYUP) {
			if (evt.key.keysym.sym == SDLK_h){
				game.hard = true;
				game.state = Game::Transition;
				game.time_passed = 0.f;
				game.capture_input = false;
				if (current != nullptr) {
					game.current->stop();
					game.current = nullptr;	
				}
				return true;
			}
			if (evt.key.keysym.sym == SDLK_n){
				game.hard = false;
				game.state = Game::Transition;	
				game.time_passed = 0.f;
				game.capture_input = false;
				if (current != nullptr) {
					game.current->stop();
					game.current = nullptr;	
				}
				return true;
			}
		}	
		return false;
	}

	if (evt.type == SDL_KEYUP) {
		switch(evt.key.keysym.sym) {
			case SDLK_a:
			case SDLK_b:
			case SDLK_c:
			case SDLK_d:
			case SDLK_e:
			case SDLK_f:
			case SDLK_g:
			case SDLK_h:
			case SDLK_i:
			case SDLK_j:
			case SDLK_k:
			case SDLK_l:
			case SDLK_m:
			case SDLK_n:
			case SDLK_o:
			case SDLK_p:
			case SDLK_q:
			case SDLK_r:
			case SDLK_s:
			case SDLK_t:
			case SDLK_u:
			case SDLK_v:
			case SDLK_w:
			case SDLK_x:
			case SDLK_y:
			case SDLK_z:
			case SDLK_1:
			case SDLK_2:
			case SDLK_3:
			case SDLK_4:
			case SDLK_5:
			case SDLK_6:
			case SDLK_7:
			case SDLK_8:
			case SDLK_9:
			case SDLK_0:
				if (game.match_letter(static_cast<char>(evt.key.keysym.sym))) {
					game.mark_correct();
					game.next_letter();
					if (game.word_matched() && !game.next_word()) {
						game.begin_playing_word_audio();
					} 
				}
				else {
					game.mark_incorrect();
					// Each mistake, add one to the game score
					game.score += 1.0f;
				}
				return true;
		}
	}
	return false;
}

void PlayMode::update(float elapsed) {
	if (game.game_over) {
		return;
	}
	switch(game.state) {
		case Game::Transition:
			if (game.play_transition_audio()) {
				game.time_passed = 0.f;	
				game.state = Game::Word;
			}
			break;
		case Game::Word:
			game.play_word_audio(elapsed);
			break;	
		case Game::Capture:
			for (auto& letter : game.letters) {
				if (letter.incorrect) {
					letter.incorrect_time += elapsed;	
					if (letter.incorrect_time > Game::INCORRECT_FADE) {
						letter.incorrect = false;	
						letter.incorrect_time = 0.f;
					}
				}	
			}
			game.time_passed += elapsed;
			break;
	}
}

void PlayMode::draw(glm::uvec2 const &drawable_size) {
	{ //use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		// Clear previously drawn text
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f
		));

		constexpr float text_size = 0.15f;
		// calculated from pure experimentation since it doesn't make sense 
		// logically
		constexpr float text_size_divisor_for_mid = 5.3f;
		std::string text = "";
		if (game.game_over) {
			text = "Game over!"; 
			lines.draw_text(text,
			glm::vec3(0.f - (static_cast<float>(text.length()) * text_size / text_size_divisor_for_mid), 0.f, 0.0),
			glm::vec3(text_size, 0.0f, 0.0f), glm::vec3(0.0f, text_size, 0.0f),
			Game::DEFAULT_COLOR);

			text = "Your score was: " + std::to_string(game.score); 
			lines.draw_text(text,
			glm::vec3(0.f - (static_cast<float>(text.length()) * text_size / text_size_divisor_for_mid), -.2f, 0.0),
			glm::vec3(text_size, 0.0f, 0.0f), glm::vec3(0.0f, text_size, 0.0f),
			Game::DEFAULT_COLOR);
		}
		else {
			switch(game.state) {
				case Game::Intro: 
					text = "Press H for hard difficulty, press N for normal difficulty"; 
					lines.draw_text(text,
					glm::vec3(0.f - (static_cast<float>(text.length()) * text_size / text_size_divisor_for_mid), 0.f, 0.0),
					glm::vec3(text_size, 0.0f, 0.0f), glm::vec3(0.0f, text_size, 0.0f),
					glm::u8vec4(0xff, 0xff, 0xff, 0x00));
					break;
				case Game::Transition:
					text = "Here is the next sequence..."; 
					lines.draw_text(text,
					glm::vec3(0.f - (static_cast<float>(text.length()) * text_size / text_size_divisor_for_mid), 0.f, 0.0),
					glm::vec3(text_size, 0.0f, 0.0f), glm::vec3(0.0f, text_size, 0.0f),
					glm::u8vec4(0xff, 0xff, 0xff, 0x00));
					break;
				case Game::Capture:
					uint32_t original_len = static_cast<uint32_t>(game.letters.size());
					uint32_t len = original_len * 3;
					float float_len = static_cast<float>(len);
					float offset = (float_len * text_size / text_size_divisor_for_mid) / (float_len / 2.f);
					float letter_offset = offset * 3.f;
					float initial_x = 0.f - (float_len * text_size / text_size_divisor_for_mid);
					for (uint32_t i = 0; i < original_len; ++i) {
						float x = initial_x + (letter_offset * static_cast<float>(i)) + 1.f * offset;	
						glm::u8vec4	color; 
						if (game.letters[i].incorrect) {
							assert(!game.letters[i].displayed);
							float fraction = (Game::INCORRECT_FADE - game.letters[i].incorrect_time) / Game::INCORRECT_FADE;
							float r = Game::INCORRECT_COLOR.x * (fraction) + ((float)0xff) * (1.f - fraction);
							float g = Game::INCORRECT_COLOR.y * (fraction) + ((float)0xff) * (1.f - fraction);
							float b = Game::INCORRECT_COLOR.z * (fraction) + ((float)0xff) * (1.f - fraction);
							color = glm::u8vec4((int)r, (int)g, (int)b, 0x00);
							text = "?";	
						}
						else if (game.letters[i].displayed) {
							text = std::string(1, game.letters[i].letter);
							color = Game::CORRECT_COLOR;
						}
						else if (i == game.current_selected()) {
							text = "?";	
							color = Game::SELECTED_COLOR;
						}
						else {
							text = "?";
							color = Game::DEFAULT_COLOR;
						}
						lines.draw_text(text,
						glm::vec3(x, 0.f, 0.0),
						glm::vec3(text_size, 0.0f, 0.0f), glm::vec3(0.0f, text_size, 0.0f),
						color);
					}
					break;
			}
		}
	}
	GL_ERRORS();
}

glm::vec3 PlayMode::get_leg_tip_position() {
	//the vertex position here was read from the model in blender:
	return lower_leg->make_local_to_world() * glm::vec4(-1.26137f, -11.861f, 0.0f, 1.0f);
}
