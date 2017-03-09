#ifndef MONTE_CARLO_AGENT_H
#define MONTE_CARLO_AGENT_H

#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include "tree_manager.hpp"

using ReversiNode = Node<GameState, coord>;

class MonteCarloAgent : public agent {
private:
    coord monte_carlo_tree_search(const GameState&);
    std::shared_ptr<ReversiNode> tree_policy(std::shared_ptr<ReversiNode> root, TreeManager&);
    void back_propagate(std::shared_ptr<ReversiNode>, unsigned result);
    unsigned simulate(std::shared_ptr<ReversiNode>);
    std::shared_ptr<ReversiNode> best_child(std::shared_ptr<ReversiNode> root_ptr);
    std::string node_scores_str(const std::vector<std::shared_ptr<ReversiNode> >&) const;
    std::shared_ptr<ReversiNode> winningest_node(const std::vector<std::shared_ptr<ReversiNode> >&) const;

public:
    coord pick_move(const GameState&);
    MonteCarloAgent(Piece _color)
        : agent(_color)
    {
    }
};

#endif