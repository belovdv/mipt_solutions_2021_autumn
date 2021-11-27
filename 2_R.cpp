#include <vector>
#include <cstdint>
#include <algorithm>
#include <functional>

class Graph {
public:
    using Index = uint32_t;
    using Vertexes = std::vector<Index>;
    using Edges = std::vector<Vertexes>;
    enum class Color { white, black };
    using Colors = std::vector<Color>;
    using Depth = uint32_t;

public:
    Graph(size_t size) : edges(Edges(size)) {
    }

public:
    Index size() const {
        return edges.size();
    }
    Edges get_edges() const {
        return edges;
    }
    Edges get_edges_reverse() const {
        Edges result(size());
        for (Index ver = 0; ver < size(); ++ver)
            for (auto next : edges[ver])
                result[next].push_back(ver);
        return result;
    }
    void link(Index from, Index to) {
        edges[from].push_back(to);
    }

public:
    static void dfs_order_tout(const Edges& edges, Colors& colors, Index root, Vertexes& order) {
        colors[root] = Color::black;
        for (auto next : edges[root])
            if (colors[next] == Color::white)
                dfs_order_tout(edges, colors, next, order);
        order.push_back(root);
    }

public:
    static Depth dfs_keeping_points(const Edges& edges, Colors& colors, Index root, std::vector<Depth>& height,
                                    uint32_t h, Vertexes& result) {
        colors[root] = Color::black;
        height[root] = h;
        Depth r_value = h;
        Depth up_from_down = h;
        Depth under = 0;
        bool is_undertree_ancestor_edge = true;
        for (auto next : edges[root])
            if (colors[next] == Color::black)
                r_value = std::min(r_value, height[next]);
            else {
                ++under;
                auto up_from_next = dfs_keeping_points(edges, colors, next, height, h + 1, result);
                up_from_down = std::min(up_from_down, up_from_next);
                if (up_from_next == h)
                    is_undertree_ancestor_edge = false;
            }
        if (h == 1) {
            if (under > 1)
                result.push_back(root);
        } else if (!is_undertree_ancestor_edge)
            result.push_back(root);
        return std::min(up_from_down, r_value);
    }
    Vertexes undirected_get_keeping_points() const {
        Colors colors(size());
        std::vector<Depth> heights(size());
        Vertexes result;
        for (Index root = 0; root < size(); ++root)
            if (colors[root] == Color::white)
                dfs_keeping_points(edges, colors, root, heights, 1, result);
        return result;
    }

private:
    Edges edges;
};

#include <iostream>

using std::cin;
using std::cout;
using std::vector;

void work() {
    int vertex_count, edges_count;
    cin >> vertex_count >> edges_count;
    Graph g(vertex_count);
    while (edges_count--) {
        int from, to;
        cin >> from >> to;
        g.link(from - 1, to - 1);
        g.link(to - 1, from - 1);
    }
    auto result = g.undirected_get_keeping_points();
    std::sort(result.begin(), result.end());
    cout << result.size() << '\n';
    for (auto it : result)
        cout << it + 1 << '\n';
}

int main() {
    cin.tie(0)->sync_with_stdio(0);
    work();
}