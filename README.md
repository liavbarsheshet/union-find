# C++ Union Find

Generic Union Find Based on up root tree written in c++.

---

## Declaration

Supports >= c++11:

```c++
#include "union-find.hpp"

auto uf = UNION::UnionFind<DATA>();
```

## Template

```c++
/**
 * Class: Represents the entire UNION FIND.
 * @tparam DATA - The type/class of the data.
 */
template<typename DATA>
class UNION::UnionFind {...}
```

## Enums

```c++
// Represents the data structure policy.
typedef enum {
U_NONE, U_OPTIMIZE, U_SENSITIVE_ORDER, U_BOTH
} POLICY;
```

## UNION::Member
```c++
/**
 * Constructor: Creates a node with a value.
 * @param value - T value.
 */
Member(T value) : {...}

/**
 * Copy Constructor: Creates a node from another.
 * @param member - Another node
 */
Member(const Member<T> &member){...}
```

## UNION::SetData

```c++
/**
 * Constructor: Creates an initial data.
 * @param id - Sets id.
 */
SetData(size_t id) {...};

/**
 * Copy Constructor: Creates set data from another.
 * @param data - Another set data reference.
 */
SetData(const SetData &data) {...}

/**
 * Joins between two SetData
 * @param data - Another set of data.
 */
void Join(const SetData &data) {...}
```

## UNION::UpNode ~ Item

```c++
/**
 * Constructor: Creates a new UpNode (Item)
 * @param id - Item id.
 * @param data - Item data.
 */
explicit UpNode(size_t id, DATA *data) {...}

/**
 * Copy Constructor: Creates a UpNode (Item) from another
 * @param nod - Another UpNode.
 */
UpNode(const UpNode &nod) {...}

/**
 * Destructor.
 */
~UpNode() {...}

/**
 * Gets the id of this item.
 * @return {size_t} This item id.
 */
size_t GetID() const {...}

/**
 * Gets the data of this item.
 * @return {DATA} This item data.
 */
DATA GetData() const {...}

/**
 * Gets the info of this item.
 * @return {UNION::SetData} This item info.
 */
UNION::SetData GetInfo() const {..}

friend std::ostream &operator<<(std::ostream &os, const UpNode &node) {...}
```

## UNION::UnionFind
```c++
/**
 * Constructor: Creates an empty Union Find DS.
 * @note Worst-Time Complexity: O(1).
 * @note Worst-Space Complexity: O(1).
 */
explicit UnionFind(UNION::POLICY policy = U_NONE) {...}

/**
 * Copy Constructor: Creates UF from another
 * @note Worst-Time Complexity: O(n).
 * @note Worst-Space Complexity: O(n).
 * @param uf A reference to another UF.
 */
UnionFind(const UnionFind<DATA> &uf) {...}
    
/**
 * Destructor.
 */
~UnionFind() {...}

/**
 * Make a new set and gives unique id to it.
 * @note Worst-Time Complexity: O(1).
 * @note Worst-Space Complexity: O(1).
 * @param data - The data that the item will have.
 * @return {size_t} Returns the new id.
 */
size_t MakeSet(DATA *data = NULL) {...};

/**
 * Find a set with a given set id.
 * @note Worst-Time Complexity: O(1).
 * @note Worst-Space Complexity: O(1).
 * @param id - The id of the set.
 * @return {UNION::UpNode<DATA>} Return the set node.
 */
UNION::UpNode<DATA> GetSet(size_t id) const {...}

/**
 * Gets the total sets that are in the UF.
 * @note It will return the number of sets not the number of items.
 * @note Worst-Time Complexity: O(1).
 * @note Worst-Space Complexity: O(1).
 * @return {size_t} Total amount of sets.
 */
size_t GetSetsAmount() const {...}

/**
 * Gets the total items that are in the UF.
 * @note It will return the number of items not the number of sets.
 * @note Worst-Time Complexity: O(1).
 * @note Worst-Space Complexity: O(1).
 * @return {size_t} Total amount of items.
 */
size_t GetItemsAmount() const {...}


/**
 * Gets a set with a given item id.
 * @note Worst-Time Complexity: O(n).
 * @note Amortized-Time with Join O(log(n)) or O(log*(n)) [IF POLICY == U_OPTIMIZE || U_BOTH].
 * @note Worst-Space Complexity: O(|path|).
 * @param id - The id of the item that belongs to a set.
 * @return {UNION::UpNode<DATA>} Return the set node.
 */
UNION::UpNode<DATA> FindSet(size_t id) {...}

/**
 * Gets an item with a given item id.
 * @param id - The id of an item.
 * @return {UNION::UpNode<DATA>} Return the item node.
 */
UNION::UpNode<DATA> GetItem(size_t id) const {...}

/**
 * Joins two sets into one set while following the rule of join small set under big set.
 * @note If policy is set to U_U_SENSITIVE_ORDER || U_BOTH, it will save the given arguments order.
 * @note Worst-Time Complexity: O(1).
 * @note Worst-Space Complexity: O(1).
 * @param p - First set id.
 * @param q - Second set id.
 */
void Join(size_t p, size_t q) {...}

/**
 * Checks whether a set is exists.
 * @note Worst-Time Complexity: O(1).
 * @note Worst-Space Complexity: O(1).
 * @param set_id - An id of a known set.
 * @returns {bool} True if exits o.w False.
 */
bool SetExists(size_t set_id) const {...}

/**
 * Checks whether an item is exists.
 * @note Worst-Time Complexity: O(1).
 * @note Worst-Space Complexity: O(1).
 * @param item_id - An id of a known set.
 * @returns {bool} True if exits o.w False.
 */
bool ItemExists(size_t item_id) const {...}

/**
 * Removes an entire set and his members(items) from UF.
 * @note Worst-Time Complexity: O(|members|).
 * @note Worst-Space Complexity: O(1).
 * @param set_id - An id of a known set.
 */
void RemoveSet(size_t set_id) {...}

/**
 * Prints the entire data structure
 * @param os - std ostream.
 * @param uf - the union find data structure.
 * @return {std::ostream} Returns ostream for chaining.
 */
friend std::ostream &operator<<(std::ostream &os, const UnionFind<DATA> &uf) {...}
```

## Author

[Liav Barsheshet, LBDevelopments](https://github.com/liavbarsheshet)

## License

[MIT](LICENSE)
