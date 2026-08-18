#include <iostream>
#include <string>
#include <chrono>

using namespace std;

/// This implementation of bubble sort is slightly different (and faster)
/// compared to the one we introduced in the readings.
/// Notice how the inner loop doesn't always go to n-1.
void bubble_sort(string items[], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (items[j] > items[j + 1]) {
                string temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }
}

bool binary_search(string items[], int n, string target) {
    int low = 0;
    int high = n - 1;

    while (low <= high) {
        int mid = (low + high) / 2;
        if (items[mid] == target) {
            return true;
        } else if (items[mid] < target) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return false;
}

bool linear_search(string items[], int n, string target) {
    for (int i = 0; i < n; i++) {
        if (items[i] == target) {
            return true;
        }
    }

    return false;
}

int user_score(bool liked, bool commented, bool shared) {
    int score = 0;

    if (liked) {
        score += 1;
    }
    if (commented) {
        score += 3;
    }
    if (shared) {
        score += 5;
    }

    if (liked && commented && shared) {
        score += 12;
    } else if (commented && shared) {
        score += 8;
    } else if (liked && shared) {
        score += 6;
    } else if (liked && commented) {
        score += 4;
    }

    return score;
}

int count_liked(string sorted_likes[], int l, string comments[], int c, string shares[], int s) {
    // Assumes the input likes array are sorted by the username.
    int score = 0;

    // Score every user who liked the post.
    for (int i = 0; i < l; i++) {
        string user = sorted_likes[i];

        bool liked = true;
        bool commented = linear_search(comments, c, user);
        bool shared = linear_search(shares, s, user);

        score += user_score(liked, commented, shared);
    }
    return score;
}

int count_comment_no_like(string sorted_likes[], int l, string comments[], int c, string shares[], int s) {
    // Assumes the input likes array are sorted by the username.
    int score = 0;

    // Score every user who commented but did not like the post.
    for (int i = 0; i < c; i++) {
        string user = comments[i];

        bool liked = binary_search(sorted_likes, l, user);

        if (!liked) {
            bool commented = true;
            bool shared = linear_search(shares, s, user);

            score += user_score(liked, commented, shared);
        }
    }
    return score;
}

int count_only_share(string sorted_likes[], int l, string comments[], int c, string shares[], int s) {
    // Assumes the input likes array are sorted by the username.
    int score = 0;

    // Score every user who shared but did not like or comment on the post.
    for (int i = 0; i < s; i++) {
        string user = shares[i];

        bool liked = binary_search(sorted_likes, l, user);
        bool commented = linear_search(comments, c, user);

        if (!liked && !commented) {
            bool shared = true;

            score += user_score(liked, commented, shared);
        }
    }
    return score;
}

int engagement_score(string likes[], int l, string comments[], int c, string shares[], int s) {
    bubble_sort(likes, l);

    int score = 0;
    score += count_liked(likes, l, comments, c, shares, s);
    score += count_comment_no_like(likes, l, comments, c, shares, s);
    score += count_only_share(likes, l, comments, c, shares, s);

    return score;
}

void test_execution_time(int n) {
    string* likes = new string[n];
    string* comments = new string[n];
    string* shares = new string[n];

    for (int i = 0; i < n; i++) {
        likes[i] = "user" + to_string(i);
        comments[i] = "user" + to_string(i);
        shares[i] = "user" + to_string(i);
    }

    auto start = chrono::high_resolution_clock::now();

    int score = engagement_score(
        likes, n,
        comments, n,
        shares, n
    );

    auto end = chrono::high_resolution_clock::now();

    auto duration =
        chrono::duration_cast<chrono::microseconds>(end - start);

    cout << "Input size n = " << n
         << ", Execution time = "
         << duration.count()
         << " microseconds"
         << endl;

    delete[] likes;
    delete[] comments;
    delete[] shares;
}

int main() {
    /*
    string likes[4] = {"Nora", "Ethan", "Maya", "Liam"};
    string comments[3] = {"Ethan", "Maya", "Zara"};
    string shares[3] = {"Nora", "Maya", "Oscar"};
    int l = 4;
    int c = 3;
    int s = 3;
    int score = engagement_score(likes, l, comments, c, shares, s);
    cout << "Engagement score for this post is " << score << endl;
    return 0;
    */

    test_execution_time(100);
    test_execution_time(500);
    test_execution_time(1000);
    test_execution_time(2000);
    test_execution_time(4000);
}