#ifndef TREE_MANAGER_H
#define TREE_MANAGER_H

#include "board.hpp"
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>

// GameState
struct GameState {
    Board board;
    Piece player_turn;
    std::vector<coord> legal_moves;
    inline bool operator==(const GameState&);

    GameState(const GameState& other)
        : board(other.board)
        , player_turn(other.player_turn)
        , legal_moves(other.legal_moves)
    {
    }

    GameState(GameState&& other)
        : board(other.board)
        , player_turn(other.player_turn)
        , legal_moves(std::move(other.legal_moves))
    {
    }

    GameState(Board _board, std::vector<coord> legal_moves, Piece _player_turn)
        : board(_board)
        , player_turn(_player_turn)
    {
    }
};

inline bool GameState::operator==(const GameState& other)
{
    return this->board == other.board && this->player_turn == other.player_turn;
}

namespace std {
template <>
struct hash<GameState> {
    std::size_t operator()(GameState const& gs) const
    {
        std::size_t h1 = std::hash<Board>{}(gs.board);
        std::size_t h2 = std::hash<Piece>{}(gs.player_turn);
        return h1 ^ (h2 << 1);
    }
};
}

// Node
struct Node {
    unsigned plays;
    unsigned wins;
    Node* parent;
    GameState game_state;
    std::vector<std::shared_ptr<Node> > children;
    std::unordered_set<coord> moves_tried;

    Node(GameState&& _game_state, Node* _parent = nullptr)
        : game_state(std::move(_game_state))
        , parent(_parent)
        , plays(0)
        , wins(0)
        , children(std::vector<std::shared_ptr<Node> >{})
        , moves_tried(std::unordered_set<coord>{})
    {
    }

    void add_child(std::shared_ptr<Node> child_ptr)
    {
        children.push_back(child_ptr);
    }
};

// TreeManager
class TreeManager {
    std::unordered_map<std::shared_ptr<GameState>, std::shared_ptr<Node> >
        state_node;

public:
    TreeManager();
    std::shared_ptr<Node> add_node(GameState&&, const coord&, Node&);
    std::shared_ptr<Node> add_root_node(GameState&&);
    std::shared_ptr<Node> get_node(GameState&&);
};

#endif