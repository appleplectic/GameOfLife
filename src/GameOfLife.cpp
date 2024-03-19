#include <SFML/Graphics.hpp>

#include <vector>
#include <random>


// get the value at a position, or if that position doesn't exist, false
bool get_val_or_false(const std::vector<std::vector<bool>>& p_grid, const int index_y, const int index_x)
{
	// assume p_grid sizes are the same
	if ((index_y >= 0) && (index_x >= 0) && (index_y < static_cast<int>(p_grid.size())) && (index_x < static_cast<int>(p_grid.at(0).size())))
	{
		const bool val = p_grid.at(index_y).at(index_x);
		return val;
	}
	else
		return false;

}


// get a random bool, true/false
bool random_bool() {
	static std::default_random_engine generator(std::random_device{}());
	static std::bernoulli_distribution distribution(0.5);
	return distribution(generator);
}


// get a 2D vector of the middle portion of a 2D vector
static std::vector<std::vector<bool>> get_middle_subgrid(const std::vector<std::vector<bool>>& g_grid, const int width, const int height)
{
	const int half_w = static_cast<int>(g_grid.at(0).size()) / 2; // assume all are the same size
	const int half_h = static_cast<int>(g_grid.size()) / 2;

	std::vector<std::vector<bool>> r{};

	for (int i = half_h - (height / 2); i < half_h + (height / 2); ++i)
	{
		std::vector<bool> w{};
		for (int j = half_w - (width / 2); j < half_w + (width / 2); ++j)
		{
			w.push_back(g_grid.at(i).at(j));
		}
		r.push_back(w);
	}

	return r;
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(1080, 720), "Game of Life");

	int fps_limit = 10;
	window.setFramerateLimit(fps_limit);

	// initalize the board, with a subsection being random bools to start the game
	int count = 300;
	std::vector grid(count, std::vector<bool>(count, false));
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			grid.at(count / 2 - 10 + i).at(count / 2 - 10 + j) = random_bool();
		}
	}

	while (window.isOpen())
	{
		// first poll for events - window & keyboard
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visible_area(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
				window.setView(sf::View(visible_area));
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
			{
				// re-initalize the board
				std::vector new_grid(count, std::vector<bool>(count, false));
				grid = new_grid;
				for (int i = 0; i < 20; ++i)
				{
					for (int j = 0; j < 20; ++j)
					{
						grid.at(count / 2 - 10 + i).at(count / 2 - 10 + j) = random_bool();
					}
				}
			}
			// increase or decrease FPS & game speed
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			{
				fps_limit += 5;
				window.setFramerateLimit(fps_limit);
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			{
				fps_limit -= 5;
				if (fps_limit < 1)
					fps_limit = 1;
				window.setFramerateLimit(fps_limit);
			}
		}

		window.clear(sf::Color::Black);

		// now run the rules of the game of life
		// to apply all rules simultaniously, we must create an old board
		auto old_grid = grid;
		int yy = 0;
		for (auto& row : grid)
		{
			int xx = 0;
			for (auto current_cell : row)
			{
				bool right = get_val_or_false(old_grid, yy, xx + 1);
				bool left = get_val_or_false(old_grid, yy, xx - 1);
				bool right_top = get_val_or_false(old_grid, yy + 1, xx + 1);
				bool top = get_val_or_false(old_grid, yy + 1, xx);
				bool left_top = get_val_or_false(old_grid, yy + 1, xx - 1);
				bool bottom = get_val_or_false(old_grid, yy - 1, xx);
				bool right_bottom = get_val_or_false(old_grid, yy - 1, xx + 1);
				bool left_bottom = get_val_or_false(old_grid, yy - 1, xx - 1);

				std::vector neighbors{ right, left, right_top, top, left_top, bottom, right_bottom, left_bottom };

				int num_neighbors = 0;
				for (auto neighbor : neighbors)
				{
					if (neighbor)
						++num_neighbors;
				}

				if (current_cell)
				{
					if ((num_neighbors < 2) || num_neighbors > 3)
						grid.at(yy).at(xx) = false;
				}
				else if (num_neighbors == 3)
					grid.at(yy).at(xx) = true;

				++xx;
			}
			++yy;
		}

		// if the window was resized, resize the vectors
		sf::Vector2u window_size = window.getSize();
		unsigned int tile_size = 30;
		unsigned int num_tiles_x = window_size.x / tile_size;
		unsigned int num_tiles_y = window_size.y / tile_size;

		// create a subgrid of the main grid that will be displayed to the screen
		auto visible_grid = get_middle_subgrid(grid, static_cast<int>(num_tiles_x), static_cast<int>(num_tiles_y));

		for (unsigned int y = 0; y < visible_grid.size(); ++y) {
			for (unsigned int x = 0; x < visible_grid[0].size(); ++x) {
				if (visible_grid.at(y).at(x)) {
					sf::RectangleShape tile(sf::Vector2f(static_cast<float>(tile_size - 1), static_cast<float>(tile_size - 1)));
					tile.setPosition(static_cast<float>(x * tile_size), static_cast<float>(y * tile_size));
					tile.setFillColor(sf::Color::White);
					window.draw(tile);
				}
			}
		}

		window.display();
	}

	return 0;
}