#ifndef MONTE_CARLO_AGENT_H
#define MONTE_CARLO_AGENT_H

#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include "tree_manager.hpp"

class MonteCarloAgent : public agent {
private:
    coord monte_carlo_tree_search(const GameState&);
    std::shared_ptr<Node> tree_policy(std::shared_ptr<Node> root, TreeManager&);
    void back_propagate(std::shared_ptr<Node>, unsigned result);
    unsigned simulate(std::shared_ptr<Node>);
    std::shared_ptr<Node> best_child(std::shared_ptr<Node> root_ptr);

public:
    coord pick_move(const GameState&);
    MonteCarloAgent(Piece _color)
        : agent(_color)
    {
    }
};

#endif