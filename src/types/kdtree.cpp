#include "hilma/types/kdtree.h"

#include <stack>

namespace hilma {

template<typename HullType> 
void KDTree<HullType>::construct() {
    nodes.clear();
    hulls.clear();
    if (lst.empty()) return;

    std::iota(lst.begin(), lst.end(), 0);

    // Determine bounding box of considered domain.
    BoundingBox box;
    for (size_t i = 0u; i < lst.size(); ++i)
        box.expand(entityPosition(i));

    auto ni = addNode(0, static_cast<unsigned int>(lst.size()));
    construct(ni, box, 0, static_cast<unsigned int>(lst.size()));
}

template<typename HullType> 
void KDTree<HullType>::construct(unsigned int node, const BoundingBox& box, unsigned int b, unsigned int n) {
    // If only one element is left end recursion.
    //if (n == 1) return;
    if (n < 10) return;

    // Determine longest side of bounding box.
    int max_dir = 0;
    glm::vec3 d = box.getDiagonal();
    if (d[1] >= d[0] && d[1] >= d[2])
        max_dir = 1;
    else if (d[2] >= d[0] && d[2] >= d[1])
        max_dir = 2;

#ifdef _DEBUG
    for (auto i = 0u; i < n; ++i)
    {
        if (!box.contains(entityPosition(lst[b + i])))
            std::cerr << "ERROR: Bounding box wrong!" << std::endl;
    }
#endif

    // Sort range according to center of the longest side.
    std::sort(lst.begin() + b, lst.begin() + b + n, 
        [&](unsigned int a, unsigned int b) {
            return entityPosition(a)(max_dir) < entityPosition(b)(max_dir);
        }
    );

    auto hal = n / 2;
    auto n0 = addNode(b      , hal    );
    auto n1 = addNode(b + hal, n - hal);
    nodes[node].children[0] = n0;
    nodes[node].children[1] = n1;

    auto c = 0.5 * (
        entityPosition(lst[b + hal -1])(max_dir) + 
        entityPosition(lst[b + hal   ])(max_dir));
    auto l_box = box; l_box.max()(max_dir) = c;
    auto r_box = box; r_box.min()(max_dir) = c;

    construct(nodes[node].children[0], l_box, b, hal);
    construct(nodes[node].children[1], r_box, b + hal, n - hal);
}

template<typename HullType>
void KDTree<HullType>::traverseDepthFirst(TraversalPredicate pred, TraversalCallback cb, const TraversalPriorityLess& pless) const {
    if (nodes.empty())
        return;

    if (pred(0, 0)) 
        traverseDepthFirst(0, 0, pred, cb, pless);
}

template<typename HullType> 
void KDTree<HullType>::traverseDepthFirst(unsigned int node_index, unsigned int depth, TraversalPredicate pred, TraversalCallback cb, const TraversalPriorityLess& pless) const {

    //auto pending = std::stack<QueueItem>{};
    //pending.push({node_index, depth});
    //while (!pending.empty())
    //{
    //    auto n = pending.top().n;
    //    auto d = pending.top().d;
    //    auto const& node = nodes[n];
    //    pending.pop();

    //    cb(n, d);
    //    auto is_pred = pred(n, d);
    //    if (!node.is_leaf() && is_pred)
    //    {
    //        if (pless && !pless(node.children))
    //        {
    //            pending.push({ static_cast<unsigned int>(node.children[1]), d + 1 });
    //            pending.push({ static_cast<unsigned int>(node.children[0]), d + 1 });
    //        }
    //        else
    //        {
    //            pending.push({ static_cast<unsigned int>(node.children[0]), d + 1 });
    //            pending.push({ static_cast<unsigned int>(node.children[1]), d + 1 });
    //        }
    //    }
    //}


    Node const& node = nodes[node_index];

    cb(node_index, depth);
    auto is_pred = pred(node_index, depth);
    if (!node.isLeaf() && is_pred)
    {
        if (pless && !pless(node.children))
        {
            traverseDepthFirst(nodes[node_index].children[1], depth + 1, pred, cb, pless);
            traverseDepthFirst(nodes[node_index].children[0], depth + 1, pred, cb, pless);
        }
        else
        {
            traverseDepthFirst(nodes[node_index].children[0], depth + 1, pred, cb, pless);
            traverseDepthFirst(nodes[node_index].children[1], depth + 1, pred, cb, pless);
        }
    }




        //    auto n = pending.front().n;
        //auto d = pending.front().d;
        //auto const& node = nodes[n];
        //pending.pop();

        //cb(n, d);
        //auto is_pred = pred(n, d);
        //if (!node.is_leaf() && is_pred)
        //{
        //    if (pless && !pless(node.children))
        //    {
        //        pending.push({ static_cast<unsigned int>(node.children[1]), d + 1 });
        //        pending.push({ static_cast<unsigned int>(node.children[0]), d + 1 });
        //    }
        //    else
        //    {
        //        pending.push({ static_cast<unsigned int>(node.children[0]), d + 1 });
        //        pending.push({ static_cast<unsigned int>(node.children[1]), d + 1 });
        //    }
        //}
}


template <typename HullType>
void KDTree<HullType>::traverseBreadthFirst(const TraversalPredicate& pred, const TraversalCallback& cb, unsigned int start_node, const TraversalPriorityLess& pless, TraversalQueue& pending) const {
    //auto pending = TraversalQueue{};
    cb(start_node, 0);
    if (pred(start_node, 0)) pending.push({ start_node, 0 });
    traverseBreadthFirst(pending, pred, cb, pless);
}

template <typename HullType> 
unsigned int KDTree<HullType>::addNode(unsigned int b, unsigned int n) {
    HullType hull;
    computeHull(b, n, hull);
    hulls.push_back(hull);
    nodes.push_back({ b, n });
    return static_cast<unsigned int>(nodes.size() - 1);
}

template <typename HullType>
void KDTree<HullType>::update() {
    traverseDepthFirst(
        [&](unsigned int, unsigned int) { return true; },
        [&](unsigned int node_index, unsigned int) {
            auto const& node = node(node_index);
            computeHull(node.begin, node.n, hull(node_index));
        }
    );
}

template <typename HullType> 
void KDTree<HullType>::traverseBreadthFirst(TraversalQueue& pending, const TraversalPredicate& pred, const TraversalCallback& cb, const TraversalPriorityLess& pless) const {
    while (!pending.empty()) {
        auto n = pending.front().n;
        auto d = pending.front().d;
        auto const& node = nodes[n];
        pending.pop();

        cb(n, d);
        auto is_pred = pred(n, d);
        if (!node.is_leaf() && is_pred) {
            if (pless && !pless(node.children)) {
                pending.push({ static_cast<unsigned int>(node.children[1]), d + 1 });
                pending.push({ static_cast<unsigned int>(node.children[0]), d + 1 });
            }
            else {
                pending.push({ static_cast<unsigned int>(node.children[0]), d + 1 });
                pending.push({ static_cast<unsigned int>(node.children[1]), d + 1 });
            }
        }
    }
}

}