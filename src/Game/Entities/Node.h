#pragma once

#include <memory>
#include "glm/glm.hpp"

class Node
{
public:
    Node(glm::vec3 position)
    {
        Position = position;

        m_rightNeighbor = nullptr;
        m_leftNeighbor = nullptr;
        m_topNeighbor = nullptr;
        m_bottomNeighbor = nullptr;
    }

    enum MovementDirection
    {
        MoveRight,
        MoveLeft,
        MoveUp,
        MoveDown
    };

    std::shared_ptr<Node> GetNeighborNode(
        MovementDirection movementDirection)
    {
        switch(movementDirection)
        {
            case(MovementDirection::MoveRight):
                return m_rightNeighbor;
            case(MovementDirection::MoveLeft):
                return m_leftNeighbor;
            case(MovementDirection::MoveUp):
                return m_topNeighbor;
            case(MovementDirection::MoveDown):
                return m_bottomNeighbor;
        }

        return nullptr;
    }

    void SetRightNeighbor(std::shared_ptr<Node>& node)
    {
        m_rightNeighbor = node;
    }

    void SetLeftNeighbor(std::shared_ptr<Node>& node)
    {
        m_leftNeighbor = node;
    }

    void SetTopNeighbor(std::shared_ptr<Node>& node)
    {
        m_topNeighbor = node;
    }

    void SetBottomNeighbor(std::shared_ptr<Node>& node)
    {
        m_bottomNeighbor = node;
    }

    glm::vec3 Position;

private:
    std::shared_ptr<Node> m_rightNeighbor;
    std::shared_ptr<Node> m_leftNeighbor;
    std::shared_ptr<Node> m_topNeighbor;
    std::shared_ptr<Node> m_bottomNeighbor;
};