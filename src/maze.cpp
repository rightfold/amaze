#include "maze.hpp"

#include <cstddef>

using namespace amaze::maze;

Maze::Maze(char const* source)
{
    width_ = -1;
    for (std::size_t i = 0; source[i] != '\0'; ++i)
    {
        if (source[i] == '\n')
        {
            if (width_ == -1)
            {
                width_ = i;
            }
            continue;
        }
        tiles_.push_back(static_cast<Tile>(source[i]));
    }
}

int Maze::size() const
{
    return tiles_.size();
}

int Maze::width() const
{
    return width_;
}

int Maze::height() const
{
    return tiles_.size() / width_;
}

Tile* Maze::tiles()
{
    return tiles_.data();
}

Tile const* Maze::tiles() const
{
    return tiles_.data();
}
