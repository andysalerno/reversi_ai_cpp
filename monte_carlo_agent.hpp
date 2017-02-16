#ifndef MONTE_CARLO_AGENT_H
#define MONTE_CARLO_AGENT_H

#include "agent.hpp"
#include "board.hpp"
#include "coord.hpp"
#include "tree_manager.hpp"
#include <memory>
#include <vector>

class monte_carlo_agent : public agent {
    coord mcts(GameState&&, std::vector<coord>&);
    std::shared_ptr<Node> tree_policy(std::shared_ptr<Node>);
    std::shared_ptr<Node> best_child(std::shared_ptr<Node>);
    int simulate(GameState);
    void back_prop(Node&, int);
    TreeManager tree_manager;

public:
    monte_carlo_agent(Piece);
    coord pick_move(const Board&, std::vector<coord>& legal_moves);
};

#endif