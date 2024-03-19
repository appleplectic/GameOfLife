#include <SFML/Graphics.hpp>

#include <deque>
#include <random>


bool get_val_or_false(const std::deque<std::deque<bool>>& p_grid, const int index_y, const int index_x)
{
	try
	{
		const bool val = p_grid.at(index_y).at(index_x);
		return val;
	}
	catch (std::out_of_range&)
	{
		return false;
	}
}


bool random_bool() {
	static std::default_random_engine generator(std::random_device{}());
	static std::bernoulli_distribution distribution(0.5);
	return distribution(generator);
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game of Life");

	window.setFramerateLimit(2);

	// initalize the grid, using std::deque<bool> instead of std::vector<bool>
	std::deque<bool> init;
	std::deque<bool> init2;
	std::deque<bool> init3;
	std::deque<bool> empty;
	for (int i = 0; i < 30; ++i)
	{
		if (i < 10)
		{
			init.push_back(false);
			init2.push_back(false);
			init3.push_back(false);
		}

		init.push_back(random_bool());
		init2.push_back(random_bool());
		init3.push_back(random_bool());
	}
	std::deque<std::deque<bool>> grid{ empty, empty, empty, empty, empty, empty, empty, empty, empty, empty, init, init2, init3 };

	while (window.isOpen())
	{
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::Resized)
			{
				sf::FloatRect visible_area(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height));
				window.setView(sf::View(visible_area));
			}
		}

		window.clear(sf::Color::Black);

		// if the window was resized, resize the vectors
		sf::Vector2u window_size = window.getSize();
		unsigned int tile_size = 30;
		unsigned int num_tiles_x = window_size.x / tile_size;
		unsigned int num_tiles_y = window_size.y / tile_size;

		grid.resize(num_tiles_y);
		for (auto& row : grid)
		{
			row.resize(num_tiles_x);
		}


		// now run the rules of the game of life
		// to apply all rules simultaniously, we must create an old board
		auto old_grid = grid;
		int yy = 0;
		for (auto& row : grid)
		{
			int xx = 0;
			for (auto current_cell : row)
			{
				bool right = get_val_or_false(grid, yy, xx + 1);
				bool left = get_val_or_false(grid, yy, xx - 1);
				bool right_top = get_val_or_false(grid, yy + 1, xx + 1);
				bool top = get_val_or_false(grid, yy + 1, xx);
				bool left_top = get_val_or_false(grid, yy + 1, xx - 1);
				bool bottom = get_val_or_false(grid, yy - 1, xx);
				bool right_bottom = get_val_or_false(grid, yy - 1, xx + 1);
				bool left_bottom = get_val_or_false(grid, yy - 1, xx - 1);

				std::deque<bool> neighbors{ right, left, right_top, top, left_top, bottom, right_bottom, left_bottom };

				int num_neighbors = 0;
				for (auto neighbor : neighbors)
				{
					if (neighbor)
						++num_neighbors;
				}

				if (current_cell)
				{
					if ((num_neighbors < 2) || num_neighbors > 3)
						grid[yy][xx] = false;
					else if ((num_neighbors == 2) || (num_neighbors == 3))
						grid[yy][xx] = true;
				}
				else if (num_neighbors == 3)
					grid[yy][xx] = true;

				++xx;
			}
			++yy;
		}


		for (unsigned int y = 0; y < num_tiles_y; ++y) {
			for (unsigned int x = 0; x < num_tiles_x; ++x) {
				if (grid[y][x]) {
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