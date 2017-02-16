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
    std::vector<coord> legal_moves;
    Piece player_turn;
    inline bool operator==(const GameState&);

    GameState(const GameState& other)
        : board(other.board)
        , legal_moves(other.legal_moves)
        , player_turn(other.player_turn)
    {
    }

    GameState(GameState&& other)
        : board(other.board)
        , legal_moves(std::move(other.legal_moves))
        , player_turn(other.player_turn)
    {
    }

    GameState(const Board& _board, std::vector<coord>& legal_moves, Piece _player_turn)
        : board(_board)
        , legal_moves(legal_moves)
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
    GameState game_state;
    Node* parent;
    unsigned plays, wins;
    std::vector<std::shared_ptr<Node> > children;
    std::unordered_set<coord> moves_tried;
    coord move; // the move that led from parent to this child

    Node(GameState&& _game_state, coord _move, Node* _parent)
        : game_state(std::move(_game_state))
        , parent(nullptr)
        , plays(0)
        , wins(0)
        , children(std::vector<std::shared_ptr<Node> >{})
        , moves_tried(std::unordered_set<coord>{})
        , move(_move)
    {
    }

    Node(GameState&& _game_state)
        : Node(std::move(_game_state), { 0, 0 }, nullptr)
    {
    }

    void add_child(std::shared_ptr<Node> child_ptr)
    {
        children.push_back(child_ptr);
    }
};

namespace std {
template <>
struct hash<Node> {
    std::size_t operator()(Node const& node) const
    {
        return std::hash<GameState>{}(node.game_state);
    }
};
}

// TreeManager
class TreeManager {
    std::unordered_map<std::shared_ptr<GameState>, std::shared_ptr<Node> >
        state_node;

public:
    TreeManager();
    std::shared_ptr<Node> add_node(GameState&&, const coord&, Node*);
    std::shared_ptr<Node> add_root_node(GameState&&);
    std::shared_ptr<Node> get_node(GameState&&);
};

#endif