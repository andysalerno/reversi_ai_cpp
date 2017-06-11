#ifndef CACHEDICT_H
#define CACHEDICT_H

template <typename K, typename V>
class CacheDict {
    K keys[2];
    V vals[2];
    unsigned size = 0;
    bool flip = true;

public:
    CacheDict() {}

    V* get(const K& key)
    {
        if (size >= 1 && key == keys[0]) {
            return &vals[0];
        } else if (size == 2 && key == keys[1]) {
            return &vals[1];
        }

        return nullptr;
    }

    void save(K key, V val)
    {
        std::size_t index = 0;
        if (flip && size >= 2) {
            index = 1;
        }

        keys[index] = key;
        vals[index] = val;

        size = size < 2 ? ++size : size;
        flip = !flip;
    }
};

#endif