//
// Created by Yihao Yu on 2/9/2026.
//


#ifndef FIT2085_DATA_STRUCTURES_H
#define FIT2085_DATA_STRUCTURES_H

#include <iostream>
#include <string>
using namespace std;

struct Post {
    int likes;
    string post_name;

    // ...
    Post()
    {
        this->likes = 0;
        this->post_name = "";
    }

    Post(int likes, string post_name)
    {
        this->likes = likes;
        this->post_name = post_name;
    }

    bool operator<(const Post& rhs) const
    {
        return this->likes < rhs.likes;
    }

    bool operator<=(const Post& rhs) const
    {
        return this->likes <= rhs.likes;
    }

    bool operator>(const Post& rhs) const
    {
        return this->likes > rhs.likes;
    }

    bool operator>=(const Post& rhs) const
    {
        return this->likes >= rhs.likes;
    }

    bool operator==(const Post& rhs) const
    {
        return this->likes == rhs.likes;
    }
};

/*
struct Batch
{
    Post* posts;
    int size;

    Batch()
    {
        this->posts = nullptr;
        this->size = 0;
    }

    Batch(Post* posts, int size)
    {
        this->posts = new Post[size];
        this->size = size;

        for (int i = 0; i < size; i++)
        {
            this->posts[i] = posts[i];
        }
    }

    Batch(const Batch& batch)
    {
        this->size = batch.size;
        this->posts = new Post[size];
        for (int i = 0; i < size; i++)
        {
            this->posts[i] = batch.posts[i];
        }
    }

    Batch& operator=(const Batch& batch)
    {
        if (this != &batch)
        {
            delete[] this->posts;

            this->size = batch.size;
            this->posts = new Post[size];
            for (int i = 0; i < size; i++)
            {
                this->posts[i] = batch.posts[i];
            }

            return *this;
        }else
        {
            return *this;
        }
    }

    ~Batch()
    {
        delete[] posts;
    }

    void remove_post(int index)
    {
        int old_size = size;
        this->size = size-1;
        Post* new_posts = new Post[size];
        for (int i = 0; i < old_size; i++)
        {
            if (i < index)
            {
                new_posts[i] = this->posts[i];
            }
            else if (i > index)
            {
                new_posts[i-1] = this->posts[i];
            }
        }
        delete[] this->posts;
        this->posts = new_posts;
    }


};
*/

template<typename T>
struct Node
{
    T item;
    Node* next;
};

template<typename T>
struct LinkedList
{
    Node<T>* first_node = nullptr;
    Node<T>* last_node = nullptr;
    int size = 0;

    void append(const T& item)
    {
        Node<T>* new_node = new Node<T>;
        new_node->item = item;
        new_node->next = nullptr;

        if (first_node == nullptr)
        {
            first_node = new_node;
            last_node = new_node;
            size++;
            return;
        }else
        {
            last_node->next = new_node;
            last_node = new_node;
            size++;
        }
    }

    ~LinkedList()
    {
        Node<T>* current = first_node;

        while (current != nullptr)
        {
            Node<T>* next = current->next;
            delete current;
            current = next;
        }
    }
};

template<typename T>
struct Batch_Node
{
    T item;
    Batch_Node<T>* left;
    Batch_Node<T>* right;
};

template<typename T>
struct Batch
{
    Batch_Node<T>* root;
    int size;

    Batch()
    {
        this->root = nullptr;
        this->size = 0;
    }

   Batch_Node<T>* build_balanced_tree(T* items, int left, int right)
    {
        if (left > right)
        {
            return nullptr;
        }

        int mid = (left + right) / 2;

        Batch_Node<T>* node = new Batch_Node<T>;
        node->item = items[mid];

        node->left = build_balanced_tree(items, left, mid - 1);
        node->right = build_balanced_tree(items, mid + 1, right);

        return node;
    }

    Batch(T* items, int size)
    {
        this->size = size;

        this->root = build_balanced_tree(items, 0, size - 1);
    }

    Batch_Node<T>* copy_batch(Batch_Node<T>* node)
    {
        if (node == nullptr)
        {
            return nullptr;
        }

        Batch_Node<T>* new_node = new Batch_Node<T>;
        new_node->item = node->item;

        new_node->left = copy_batch(node->left);
        new_node->right = copy_batch(node->right);

        return new_node;
    }

    Batch(const Batch<T>& batch)
    {
        this->size = batch.size;
        this->root = copy_batch(batch.root);
    }

    void delete_batch(Batch_Node<T>* node)
    {
        if (node == nullptr)
        {
            return;
        }

        delete_batch(node->right);
        delete_batch(node->left);

        delete node;
    }

    ~Batch()
    {
        delete_batch(root);
    }

    Batch& operator=(const Batch& batch)
    {
        if (this != &batch)
        {
            delete_batch(this->root);

            this->size = batch.size;
            this->root = copy_batch(batch.root);
        }

        return *this;
    }

    Batch_Node<T>* find_min(Batch_Node<T>* current_node)
    {
        while (current_node -> left != nullptr)
        {
            current_node = current_node -> left;
        }

        return current_node;
    }

    Batch_Node<T>* delete_helper(Batch_Node<T>* current_node, T item)
    {
        if (current_node == nullptr)
        {
            return nullptr;
        }

        if (item < current_node->item)
        {
            current_node->left = delete_helper(current_node->left, item);

            return current_node;
        }

        if (item > current_node->item)
        {
            current_node->right = delete_helper(current_node->right, item);

            return current_node;
        }

        // Found target

        // 0 children
        if (
            current_node->left == nullptr &&
            current_node->right == nullptr)
        {
            delete current_node;
            return nullptr;
        }

        // only right children
        if (current_node -> left == nullptr)
        {
            Batch_Node<T>* replacement = current_node->right;
            delete current_node;
            return replacement;
        }

        // only left children
        if (current_node -> right == nullptr)
        {
            Batch_Node<T>* replacement = current_node->left;
            delete current_node;
            return replacement;
        }

        // 2 children
        Batch_Node<T>* successor = find_min(current_node->right);

        current_node -> item = successor->item;

        current_node -> right = delete_helper(current_node->right, successor->item);

        return current_node;
    }

    void delete_item(T item)
    {
        root = delete_helper(root, item);
    }

    T* least_over_threshold(T threshold)
    {
        Batch_Node<T>* current_node = root;
        T* result = nullptr;

        while (current_node != nullptr)
        {
            if (current_node->item > threshold)
            {
                result = &current_node -> item;

                current_node = current_node -> left;
            }else
            {
                current_node = current_node -> right;
            }
        }

        return result;
    }

};



#endif //FIT2085_DATA_STRUCTURES_H
