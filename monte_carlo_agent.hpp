#ifndef MONTE_CARLO_AGENT_H
#define MONTE_CARLO_AGENT_H

#include "MonteCarloTree/monte_carlo_tree.hpp"
#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"

using ReversiNode = Node<GameState, coord>;
using ReversiTree = MonteCarloTree<GameState, coord>;

class MonteCarloAgent : public agent {
private:
    ReversiTree reversi_tree;
    coord monte_carlo_tree_search(const GameState&);
    std::shared_ptr<ReversiNode> tree_policy(std::shared_ptr<ReversiNode> root);
    void back_propagate(std::shared_ptr<ReversiNode>, unsigned result);
    unsigned simulate(std::shared_ptr<ReversiNode>);
    std::shared_ptr<ReversiNode> best_child(std::shared_ptr<ReversiNode> root_ptr);
    std::string node_scores_str(const std::vector<std::shared_ptr<ReversiNode>>&) const;
    std::shared_ptr<ReversiNode> winningest_node(const std::vector<std::shared_ptr<ReversiNode>>&) const;

public:
    coord pick_move(const GameState&);
    MonteCarloAgent(Piece _color)
        : agent(_color)
    {
    }
};

#endif