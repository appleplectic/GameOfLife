#include <SFML/Graphics.hpp>

#include <vector>
#include <random>


bool get_val_or_false(const std::vector<std::vector<bool>>& p_grid, const int index_y, const int index_x)
{
	// assume p_grid sizes are the same
	if ((index_y >= 0) && (index_x >= 0) && (index_y < static_cast<int>(p_grid.size())) && (index_x < static_cast<int>(p_grid[0].size())))
	{
		const bool val = p_grid.at(index_y).at(index_x);
		return val;
	}
	else
		return false;

}


bool random_bool() {
	static std::default_random_engine generator(std::random_device{}());
	static std::bernoulli_distribution distribution(0.5);
	return distribution(generator);
}


static std::vector<std::vector<bool>> get_middle_subgrid(const std::vector<std::vector<bool>>& g_grid, const int width, const int height)
{
	const int half_w = static_cast<int>(g_grid[0].size()) / 2; // assume all are the same size
	const int half_h = static_cast<int>(g_grid.size()) / 2;

	std::vector<std::vector<bool>> r{};

	for (int i = half_h - (height / 2); i < half_h + (height / 2); ++i)
	{
		std::vector<bool> w{};
		for (int j = half_w - (width / 2); j < half_w + (width / 2); ++j)
		{
			w.push_back(g_grid[i][j]);
		}
		r.push_back(w);
	}

	return r;
}


int main()
{
	sf::RenderWindow window(sf::VideoMode(1920, 1080), "Game of Life");

	window.setFramerateLimit(16);

	int count = 300;
	std::vector<std::vector<bool>> grid(count, std::vector<bool>(count, false));
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			grid[count / 2 - 10 + i][count / 2 - 10 + j] = random_bool();
		}
	}

	while (window.isOpen())
	{
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
				std::vector<std::vector<bool>> new_grid(count, std::vector<bool>(count, false));
				grid = new_grid;
				for (int i = 0; i < 20; ++i)
				{
					for (int j = 0; j < 20; ++j)
					{
						grid[count / 2 - 10 + i][count / 2 - 10 + j] = random_bool();
					}
				}
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

				std::vector<bool> neighbors{ right, left, right_top, top, left_top, bottom, right_bottom, left_bottom };

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
				}
				else if (num_neighbors == 3)
					grid[yy][xx] = true;

				++xx;
			}
			++yy;
		}

		// if the window was resized, resize the vectors
		sf::Vector2u window_size = window.getSize();
		unsigned int tile_size = 30;
		unsigned int num_tiles_x = window_size.x / tile_size;
		unsigned int num_tiles_y = window_size.y / tile_size;

		auto visible_grid = get_middle_subgrid(grid, static_cast<int>(num_tiles_x), static_cast<int>(num_tiles_y));

		for (unsigned int y = 0; y < num_tiles_y; ++y) {
			for (unsigned int x = 0; x < num_tiles_x; ++x) {
				if (visible_grid[y][x]) {
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