#ifndef MONTE_CARLO_AGENT_H
#define MONTE_CARLO_AGENT_H

#include "MonteCarloTree/monte_carlo_tree.hpp"
#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"

using ReversiNode = Node<GameState, Coord>;
using ReversiTree = MonteCarloTree<GameState, Coord>;

class MonteCarloAgent : public Agent {
private:
    ReversiTree reversi_tree;
    Coord monte_carlo_tree_search(const GameState&);
    ReversiNode& tree_policy(ReversiNode& root);
    void back_propagate(ReversiNode&, unsigned result);
    unsigned simulate(ReversiNode&);
    ReversiNode& best_child(ReversiNode&);
    std::string node_scores_str(const std::vector<std::shared_ptr<ReversiNode> >&) const;
    ReversiNode& winningest_node(const std::vector<std::shared_ptr<ReversiNode> >&) const;

public:
    Coord pick_move(const GameState&);
    MonteCarloAgent(Piece _color)
        : Agent(_color)
    {
    }
};

#endif