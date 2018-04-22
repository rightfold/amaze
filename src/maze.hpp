#pragma once

#include <vector>

namespace amaze::maze
{
    enum class Tile : char
    {
        floor = '.',
        wall  = '#',
        lava  = '!',
    };

    class Maze
    {
    public:
        explicit Maze(char const*);

        int size() const;
        int width() const;
        int height() const;

        Tile* tiles();
        Tile const* tiles() const;

    private:
        int width_;

        std::vector<Tile> tiles_;
    };
}
