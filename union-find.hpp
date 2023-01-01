/**
 * Generic Union Find Based on up root tree.
 *
 * @file union-find.hpp
 *
 * @brief Implementation of a Generic Generic Union Find.
 *
 * @author Liav Barsheshet
 * Contact: liavbarsheshet@gmail.com
 *
 * This implementation is free: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This implementation is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 */

#include<cstdlib>
#include<iostream>
#include <vector>


#ifndef _UNION_FIND_HPP
#define _UNION_FIND_HPP

namespace UNION {

    typedef enum {
        U_NONE, U_OPTIMIZE, U_SENSITIVE_ORDER, U_BOTH
    } POLICY;

    class uf_error : public std::exception {
    };

    class uf_set_not_exists : public uf_error {
    };

    class uf_item_not_exists : public uf_error {
    };

    class SetData;

    template<typename T>
    class Member;

    template<typename DATA>
    class UpNode;

    template<typename DATA>
    class UnionFind;
}

/**
 * Class: Represents member node.
 * @tparam T Type of the value.
 */
template<typename T>
class UNION::Member {
public:
    Member *next;
    T value;

    /**
     * Constructor: Creates a node with a value.
     * @param value - T value.
     */
    Member(T value) : next(NULL), value(value) {}

    /**
     * Copy Constructor: Creates a node from another.
     * @param member - Another node
     */
    Member(const Member<T> &member) : next(member.next), value(member.value) {}
};

/**
 * Class: Represents the set data.
 */
class UNION::SetData {
public:
    size_t set_id;
    size_t size;
    UNION::Member<size_t> *members;
    UNION::Member<size_t> *last;

    /**
     * Constructor: Creates an initial data.
     * @note Worst-Time Complexity: O(1).
     * @note Worst-Space Complexity: O(1).
     * @param id - Sets id.
     */
    SetData(size_t id) :
            set_id(id),
            size(1) {
        this->members = new UNION::Member<size_t>(id);
        last = this->members;
    }

    /**
     * Copy Constructor: Creates set data from another.
     * @note Worst-Time Complexity: O(n).
     * @note Worst-Space Complexity: O(n).
     * @param data - Another set data reference.
     */
    SetData(const SetData &data) :
            set_id(data.set_id),
            size(data.size),
            members(NULL) {
        UNION::Member<size_t> *from_member = data.members;
        UNION::Member<size_t> *to_member = NULL;


        while (from_member) {
            if (to_member == NULL) {
                to_member = new UNION::Member<size_t>(from_member->value);
                this->members = to_member;
            } else {
                to_member->next = new UNION::Member<size_t>(from_member->value);
                to_member = to_member->next;
            }

            from_member = from_member->next;
        }
    }

    /**
     * Destructor.
     */
    ~SetData() {
        if (this->members == NULL) {
            return;
        }

        UNION::Member<size_t> *member = this->members;
        UNION::Member<size_t> *tmp;
        while (member) {
            tmp = member;
            member = member->next;
            delete tmp;
        }
    }

    /**
     * Joins between two SetData
     * @param data - Another set of data.
     */
    void Join(SetData &data) {
        this->size += data.size;
        this->last->next = data.members;
        this->last = data.last;
        data.members = NULL;
    }
};


/**
 * Class: Represents nodes inside the UNION FIND DS.
 * @tparam DATA - The type/class of the data.
 */
template<typename DATA>
class UNION::UpNode {
    size_t id;
    UpNode *parent;
    DATA *data;
    UNION::SetData *info;

    friend class UNION::UnionFind<DATA>;

public:
    /**
     * Constructor: Creates a new empty UpNode (Item)
     */
    UpNode() : id(0), parent(NULL), data(NULL), info(NULL) {}

    /**
     * Constructor: Creates a new UpNode (Item)
     * @param id - Item id.
     * @param data - Item data.
     */
    explicit UpNode(size_t id, DATA *data) : id(id), parent(NULL), data(data), info(new UNION::SetData(id)) {}

    /**
     * Copy Constructor: Creates a UpNode (Item) from another
     * @param nod - Another UpNode.
     */
    UpNode(const UpNode &nod) : id(nod.id), parent(NULL), data(nod.data), info(new UNION::SetData(*nod.info)) {}

    /**
     * Destructor.
     */
    ~UpNode() {
        if (!this->info) return;
        delete this->info;
    }

    /**
     * Gets the id of this item.
     * @return {size_t} This item id.
     */
    size_t GetID() const {
        return this->id;
    }

    /**
     * Gets the data of this item.
     * @return {DATA} This item data.
     */
    DATA GetData() const {
        return *(this->data);
    }

    /**
     * Gets the info of this item.
     * @return {UNION::SetData} This item info.
     */
    UNION::SetData GetInfo() const {
        if (!this->info) {
            return UNION::SetData(0);
        }
        return *(this->info);
    }

    friend std::ostream &operator<<(std::ostream &os, const UpNode &node) {

        if (node.parent != NULL) {
            os << "Item: " << node.id << std::endl;
            return os;
        }

        os << "Set[" << node.id << "] {" << std::endl;
        os << "\t\t" << "id: " << node.id << "," << std::endl;
        os << "\t\t" << "set_size: " << node.info->size << "," << std::endl;
        os << "\t\t" << "members: [";

        UNION::Member<size_t> *member = node.info->members;

        while (member) {

            if (member->next == NULL) {
                os << member->value;
            } else {
                os << member->value << ", ";
            }

            member = member->next;
        }

        os << "]" << std::endl;
        os << "\t" << "}" << std::endl;

        return os;
    }
};

/**
 * Class: Represents the entire UNION FIND.
 * @tparam DATA - The type/class of the data.
 */
template<typename DATA>
class UNION::UnionFind {
    std::vector<UNION::UpNode<DATA> *> sets;
    std::vector<UNION::UpNode<DATA> *> nodes;
    std::vector<size_t> free_ids;
    size_t sets_amount;
    size_t items_amount;

    UNION::POLICY policy;

    UNION::UpNode<DATA> *GetSetPointer(size_t id) {
        if (this->nodes.size() < id - 1 || id == 0) {
            throw UNION::uf_set_not_exists();
        }

        UNION::UpNode<DATA> *set = this->sets[id - 1];

        if (set == NULL) {
            throw UNION::uf_set_not_exists();
        }

        return set;

    }

public:
    /**
     * Constructor: Creates an empty Union Find DS.
     * @note Worst-Time Complexity: O(1).
     * @note Worst-Space Complexity: O(1).
     */
    UnionFind(UNION::POLICY policy = U_NONE) :
            sets(),
            nodes(),
            free_ids(),
            sets_amount(0),
            items_amount(0),
            policy(policy) {}

    /**
     * Copy Constructor: Creates UF from another
     * @note Worst-Time Complexity: O(n).
     * @note Worst-Space Complexity: O(n).
     * @param uf A reference to another UF.
     */
    UnionFind(const UnionFind<DATA> &uf) :
            sets(),
            nodes(),
            free_ids(),
            sets_amount(uf.sets_amount),
            items_amount(uf.items_amount),
            policy(uf.policy) {
        for (auto node: uf.nodes) {
            this->nodes.push_back((node == NULL ? NULL : new UNION::UpNode<DATA>(*node)));
        }
        for (auto set: uf.sets) {
            this->sets.push_back((set == NULL ? NULL : new UNION::UpNode<DATA>(*set)));
        }
        for (auto index: uf.free_ids) {
            this->free_ids.push_back(index);
        }
    }

    /**
     * Destructor.
     */
    ~UnionFind() {
        for (UNION::UpNode<DATA> *node: this->nodes) {
            if (node == NULL) {
                continue;
            }
            delete node;
        }
    }

    /**
     * Make a new set and gives unique id to it.
     * @note Worst-Time Complexity: O(1).
     * @note Worst-Space Complexity: O(1).
     * @param data - The data that the item will have.
     * @return {size_t} Returns the new id.
     */
    size_t MakeSet(DATA *data = NULL) {
        size_t id;

        if (this->free_ids.empty()) {
            id = this->nodes.size() + 1;
        } else {
            id = *(this->free_ids.rbegin());
            this->free_ids.pop_back();
        }

        auto new_item = new UNION::UpNode<DATA>(id, data);

        if (id - 1 == this->nodes.size()) {
            this->nodes.push_back(new_item);
            this->sets.push_back(new_item);
        } else {
            this->nodes[id - 1] = new_item;
            this->sets[id - 1] = new_item;
        }

        ++this->sets_amount;
        ++this->items_amount;

        return id;
    };

    /**
     * Find a set with a given set id.
     * @note Worst-Time Complexity: O(1).
     * @note Worst-Space Complexity: O(1).
     * @param id - The id of the set.
     * @return {UNION::UpNode<DATA>} Return the set node.
     */
    UNION::UpNode<DATA> GetSet(size_t id) const {
        if (this->nodes.size() >= id || id == 0) {
            throw UNION::uf_set_not_exists();
        }

        UNION::UpNode<DATA> *set = this->sets[id - 1];

        if (set == NULL) {
            throw UNION::uf_set_not_exists();
        }

        return *set;

    }

    /**
     * Gets the total sets that are in the UF.
     * @note It will return the number of sets not the number of items.
     * @note Worst-Time Complexity: O(1).
     * @note Worst-Space Complexity: O(1).
     * @return {size_t} Total amount of sets.
     */
    size_t GetSetsAmount() const {
        return this->sets_amount;
    }

    /**
     * Gets the total items that are in the UF.
     * @note It will return the number of items not the number of sets.
     * @note Worst-Time Complexity: O(1).
     * @note Worst-Space Complexity: O(1).
     * @return {size_t} Total amount of items.
     */
    size_t GetItemsAmount() const {
        return this->items_amount;
    }


    /**
     * Gets a set with a given item id.
     * @note Worst-Time Complexity: O(n).
     * @note Amortized-Time with Join O(log(n)) or O(log*(n)) [IF POLICY == U_OPTIMIZE || U_BOTH].
     * @note Worst-Space Complexity: O(|path|).
     * @param id - The id of the item that belongs to a set.
     * @return {UNION::UpNode<DATA>} Return the set node.
     */
    UNION::UpNode<DATA> FindSet(size_t id) {
        if (this->nodes.size() < id || id == 0) {
            throw UNION::uf_set_not_exists();
        }

        UNION::UpNode<DATA> *node = this->nodes[id - 1];

        if (node == NULL) {
            throw UNION::uf_set_not_exists();
        }

        std::vector<UNION::UpNode<DATA> *> path = std::vector<UNION::UpNode<DATA> *>();

        while (node->parent) {
            if (this->policy == U_OPTIMIZE || this->policy == U_BOTH) {
                path.push_back(node);
            }
            node = node->parent;
        }

        // Shrinking Path if Policy is set to U_OPTIMIZE.
        for (UNION::UpNode<DATA> *p_node: path) {
            p_node->parent = node;
        }

        return *node;
    }

    /**
     * Gets an item with a given item id.
     * @param id - The id of an item.
     * @return {UNION::UpNode<DATA>} Return the item node.
     */
    UNION::UpNode<DATA> GetItem(size_t id) const {
        if (this->nodes.size() < id || id == 0) {
            throw UNION::uf_item_not_exists();
        }

        UNION::UpNode<DATA> *node = this->nodes[id - 1];

        if (node == NULL) {
            throw UNION::uf_item_not_exists();
        }

        return *node;
    }

    /**
     * Joins two sets into one set while following the rule of join small set under big set.
     * @note If policy is set to U_U_SENSITIVE_ORDER || U_BOTH, it will save the given arguments order.
     * @note Worst-Time Complexity: O(1).
     * @note Worst-Space Complexity: O(1).
     * @param p - First set id.
     * @param q - Second set id.
     */
    void Join(size_t p, size_t q) {
        if (p == q) {
            return;
        }

        auto p_node = GetSetPointer(p);
        auto q_node = GetSetPointer(q);

        bool q_is_bigger = q_node->info->size > p_node->info->size;

        UNION::UpNode<DATA> *x_node = (q_is_bigger ? q_node : p_node);
        UNION::UpNode<DATA> *y_node = (q_is_bigger ? p_node : q_node);

        y_node->parent = x_node;
        x_node->info->Join(*y_node->info);

        // Deallocating info for space optimization.
        delete y_node->info;
        y_node->info = NULL;

        // Reassures that the order is saved while respecting the rule.
        if (this->policy == U_SENSITIVE_ORDER || this->policy == U_BOTH) {
            auto q_copy = UNION::UpNode<DATA>();
            auto p_copy = UNION::UpNode<DATA>();
            p_copy.id = p_node->id;
            p_copy.data = p_node->data;
            q_copy.id = q_node->id;
            q_copy.data = q_node->data;

            x_node->id = p_copy.id;
            x_node->data = p_copy.data;
            y_node->id = q_copy.id;
            y_node->data = q_copy.data;

            this->nodes[p_copy.id - 1] = x_node;
            this->nodes[q_copy.id - 1] = y_node;
            this->sets[p_copy.id - 1] = x_node;
            this->sets[q_copy.id - 1] = y_node;
        }

        this->sets[y_node->id - 1] = NULL;
        --this->sets_amount;
    }

    /**
     * Checks whether a set is exists.
     * @note Worst-Time Complexity: O(1).
     * @note Worst-Space Complexity: O(1).
     * @param set_id - An id of a known set.
     * @returns {bool} True if exits o.w False.
     */
    bool SetExists(size_t set_id) const {
        if (set_id > this->nodes.size() || set_id == 0) {
            return false;
        }

        return this->sets[set_id - 1] != NULL;
    }

    /**
     * Checks whether an item is exists.
     * @note Worst-Time Complexity: O(1).
     * @note Worst-Space Complexity: O(1).
     * @param item_id - An id of a known set.
     * @returns {bool} True if exits o.w False.
     */
    bool ItemExists(size_t item_id) const {
        if (item_id > this->nodes.size() || item_id == 0) {
            return false;
        }

        return this->nodes[item_id - 1] != NULL;
    }

    /**
     * Removes an entire set and his members(items) from UF.
     * @note Worst-Time Complexity: O(|members|).
     * @note Worst-Space Complexity: O(1).
     * @param set_id - An id of a known set.
     */
    void RemoveSet(size_t set_id) {
        UNION::UpNode<DATA> *set = this->GetSetPointer(set_id);

        UNION::Member<size_t> *member_list = set->info->members;
        std::vector<size_t> members = std::vector<size_t>();

        while (member_list) {
            members.push_back(member_list->value);
            member_list = member_list->next;
        }

        for (auto member: members) {
            UNION::UpNode<DATA> *node = this->nodes[member - 1];
            this->nodes[member - 1] = NULL;
            this->free_ids.push_back(member);
            delete node;
            --this->items_amount;
        }

        // Removes empty items at the end.
        while (*(this->nodes.rbegin()) == NULL) {
            this->nodes.pop_back();
            this->sets.pop_back();
        }

        --this->sets_amount;
    }

    /**
     * Prints the entire data structure
     * @param os - std ostream.
     * @param uf - the union find data structure.
     * @return {std::ostream} Returns ostream for chaining.
     */
    friend std::ostream &operator<<(std::ostream &os, const UnionFind<DATA> &uf) {
        os << "Union Find: {" << std::endl;

        for (UNION::UpNode<DATA> *node: uf.sets) {
            if (node == NULL) continue;
            os << "\t" << *node << std::endl;
        }

        os << "}" << std::endl;
        return os;
    }
};

#endif
