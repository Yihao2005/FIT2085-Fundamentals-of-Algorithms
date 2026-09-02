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



#endif //FIT2085_DATA_STRUCTURES_H
