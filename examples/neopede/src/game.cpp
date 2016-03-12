#include <neogfx/neogfx.hpp>
#include <neogfx/app.hpp>
#include <neogfx/i_layout.hpp>
#include <neogfx/sprite.hpp>
#include <neogfx/sprite_plane.hpp>
#include <neogfx/image.hpp>

namespace ng = neogfx;

const uint8_t sSpaceshipImagePattern[8][8]
{
	{ 0, 0, 0, 0, 1, 0, 0, 0 },
	{ 0, 0, 0, 1, 2, 1, 0, 0 },
	{ 0, 0, 0, 1, 2, 1, 0, 0 },
	{ 0, 0, 1, 2, 2, 2, 1, 0 },
	{ 0, 0, 1, 2, 2, 2, 1, 0 },
	{ 0, 1, 1, 2, 2, 2, 1, 1 },
	{ 0, 1, 0, 1, 1, 1, 0, 1 },
	{ 0, 1, 0, 0, 0, 0, 0, 1 },
};

void create_game(ng::i_layout& aLayout)
{
	auto spritePlane = std::make_shared<ng::sprite_plane>();
	aLayout.add_widget(spritePlane);
	spritePlane->set_font(ng::font(spritePlane->font(), ng::font::Bold, 28));
	spritePlane->set_background_colour(ng::colour::Black);
	spritePlane->enable_z_sorting(true);
	for (uint32_t i = 0; i < 1000; ++i)
		spritePlane->add_shape(std::make_shared<ng::rectangle>(
			ng::vec3{static_cast<ng::scalar>(std::rand() % 800), static_cast<ng::scalar>(std::rand() % 800), static_cast<ng::scalar>(std::rand() % 32)},
			ng::size{static_cast<ng::scalar>(std::rand() % 32), static_cast<ng::scalar>(std::rand() % 32)},
			ng::colour(std::rand() % 32, std::rand() % 32, std::rand() % 32)));
	//spritePlane->set_uniform_gravity();
	spritePlane->create_earth();
	auto& spaceshipSprite = spritePlane->create_sprite(ng::image(sSpaceshipImagePattern, { {0, ng::colour()}, {1, ng::colour::LightGoldenrod}, {2, ng::colour::DarkGoldenrod4} }));
	spaceshipSprite.physics().set_mass(1.0);
	spaceshipSprite.set_size(ng::size(32.0, 32.0));
	spaceshipSprite.set_position_3D(ng::vec3(400.0, 16.0, 24.0));
	spritePlane->painting([spritePlane](ng::graphics_context& aGraphicsContext)
	{
		aGraphicsContext.draw_text(ng::point(0.0, 0.0), "Hello, World!", spritePlane->font(), ng::colour::White);
	});
	spritePlane->physics_applied([&spaceshipSprite]()
	{
		const auto& keyboard = ng::app::instance().keyboard();
		spaceshipSprite.physics().set_acceleration({  
			keyboard.is_key_pressed(ng::ScanCode_RIGHT) ? 16.0 : keyboard.is_key_pressed(ng::ScanCode_LEFT) ? -16.0 : 0.0,
			keyboard.is_key_pressed(ng::ScanCode_UP) ? 16.0 : keyboard.is_key_pressed(ng::ScanCode_DOWN) ? -16.0 : 0.0});
		if (keyboard.is_key_pressed(ng::ScanCode_X))
			spaceshipSprite.physics().set_spin_degrees(30.0);
		else if (keyboard.is_key_pressed(ng::ScanCode_Z))
			spaceshipSprite.physics().set_spin_degrees(-30.0);
		else
			spaceshipSprite.physics().set_spin_degrees(0.0);
	});
}