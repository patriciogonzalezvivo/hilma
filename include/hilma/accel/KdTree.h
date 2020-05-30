#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include <numeric>
#include <queue>
#include <iostream>

#include "glm/glm.hpp"
#include "hilma/accel/BoundingBox.h"

#include <array>
#include <list>
#include <stack>

namespace hilma
{

template <typename HullType>
class KDTree {
public:

    using TraversalPredicate = std::function<bool (unsigned int node_index, unsigned int depth)> ;
    using TraversalCallback = std::function <void (unsigned int node_index, unsigned int depth)>;
    using TraversalPriorityLess = std::function<bool (std::array<int, 2> const& nodes)>;

    struct Node {
        Node(unsigned int b_, unsigned int n_)
            : children({{-1, -1}})
            , begin(b_), n(n_) {}

        Node() = default;

        bool isLeaf() const { return children[0] < 0 && children[1] < 0; }

        // Index of child nodes in nodes array.
        // -1 if child does not exist.
        std::array<int, 2> children;

        // Index according entries in entity list.
        unsigned int begin;

        // Number of owned entries.
        unsigned int n;
    };

    struct QueueItem { unsigned int n, d; };
    using TraversalQueue = std::queue<QueueItem>;

    KDTree(std::size_t n): lst(n) {}
    virtual ~KDTree() {}

    Node const& node(unsigned int i) const { return nodes[i]; }
    HullType const& hull(unsigned int i) const { return hulls[i]; }
    unsigned int entity(unsigned int i) const { return lst[i]; }

    void construct();
    void update();
    void traverseDepthFirst(TraversalPredicate pred, TraversalCallback cb, const TraversalPriorityLess& pless = nullptr) const;
    void traverseBreadthFirst(const TraversalPredicate& pred, const TraversalCallback& cb, unsigned int start_node = 0, const TraversalPriorityLess& pless = nullptr, TraversalQueue& pending = TraversalQueue()) const;

protected:

    void construct(unsigned int node, BoundingBox const& box,
        unsigned int b, unsigned int n);
    void traverseDepthFirst(unsigned int node, unsigned int depth,
        TraversalPredicate pred, TraversalCallback cb, const TraversalPriorityLess& pless) const;
    void traverseBreadthFirst(TraversalQueue& pending,
        const TraversalPredicate& pred, const TraversalCallback& cb, const TraversalPriorityLess& pless = nullptr) const;

    unsigned int addNode(unsigned int b, unsigned int n);

    virtual glm::vec3 const& entityPosition(unsigned int i) const = 0;
    virtual void computeHull(unsigned int b, unsigned int n, HullType& hull) const = 0;

protected:

    std::vector<unsigned int>   lst;
    std::vector<Node>           nodes;
    std::vector<HullType>       hulls;
};

}