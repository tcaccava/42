#include "PmergeMe.hpp"
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cassert>

using std::cout;
using std::endl;

// modified to be easier to use
ssize_t jacobsthal(unsigned int i)
{
    return ((1LL << (i+1)) + (i & 1? -1 : 1)) / 3 - 2;
}

std::vector<unsigned int> parse_vec(const std::string &s)
{
    std::vector<unsigned int> v;
    int x;
    std::stringstream ss(s);
    while(!ss.eof())
    {
        ss >> x;
        if(ss.fail() || ss.bad() || x < 0)
        {
            if(ss.eof())
                break;
            throw std::runtime_error("Error");
        }
        v.push_back(x);
    }
    return v;
}


// std::deque<unsigned int> parse_deque(const std::string &s)
// {
//     std::deque<unsigned int> v;
//     int x;
//     std::stringstream ss(s);
//     while(!ss.eof())
//     {
//         ss >> x;
//         if(ss.fail() || ss.bad() || x < 0)
//         {
//             if(ss.eof())
//                 break;
//             throw std::runtime_error("Error");
//         }
//         v.push_back(x);
//     }
//     return v;
// }

static void swap_chunk(int chsize, int base, std::vector<unsigned int> &v)
{
    for (int i = 0; i < chsize; i++)
    {
        std::swap(v[base + i], v[base+i+chsize]);
    }
}

template<class T>
static std::ostream& operator << (std::ostream &out, const std::vector<T>& vec)
{
    out << "["; 
    for(int i=0;i < int(vec.size()); i++)
        out << ' ' << vec[i]; 
    return out << " ]\n";
}


static void custom_insert(
    std::vector<unsigned int> &v,
    std::vector<unsigned int> &sorted,
    size_t pref_size,
    size_t index,
    int &comp
)
{
    unsigned int value = v[index];
    std::vector<unsigned int> sorted_values = sorted;
    for (size_t i = 0; i < sorted_values.size(); i++)
        sorted_values[i] = v[sorted_values[i]];
    // cout << "custom_insert size = " << pref_size << " value = " << value;
    // cout <<  " sorted_values = " << sorted_values;
    size_t l = 0;
    size_t r = pref_size;
    while(l < r)
    {
        int mid = (l+r)/2;
        if(value <= v[sorted[mid]])
        {
            r = mid;
        }
        else 
        {
            l = mid + 1;
        }
        comp++;
    }

    sorted.insert(sorted.begin()+l, index);
    // return l;
}

static void sort(
    std::vector<unsigned int> &v, 
    size_t chsize, 
    std::vector<unsigned int> &sorted,
    std::vector<unsigned int> &pend,
    int &comp
)
{
    size_t dchunk = chsize * 2;
    if (dchunk > v.size()) // single chunk, nothing to do
    {
        sorted.push_back(chsize - 1);
        return;
    }
    // Swap adjacent elements if they are unsorted
    // (since we want to keep the ordering in the recursion, swap the whole chunks)
    for (size_t i = 0; i + dchunk <= v.size(); i += dchunk)
    {
        int h1 = i + chsize - 1;
        int h2 = i + dchunk - 1;
        if(v[h1] > v[h2])
        {
            swap_chunk(chsize, i, v);
        }
        comp++;
    }
    // cout << v << endl;
    sort(v, chsize*2, sorted, pend, comp); // recursive part
    // now we expect that the double chunks are sorted
    // cout << "sorted = " << sorted;
    int nchunks = v.size() / chsize;

    // sorted now represents the positions of the a elements
    sorted.insert(sorted.begin(), sorted[0] ^ chsize);
    int pend_size = 0;
    for (size_t i = 2;i < sorted.size();i++)
    {
        pend[pend_size] = sorted[i] ^ chsize;
        pend_size++;
    }


    // pend.resize(pend_size);
    // cout << "sorted = " << sorted << "pend = " << pend;

    ssize_t left = -1;
    ssize_t right = 0;
    for (int k = 2; left < pend_size; k++)
    {
        right = std::min<ssize_t>(jacobsthal(k), pend_size - 1);
        // cout << k <<' '<<left <<' '<< right << ' ' <<pend_size << endl;
        while(right > left)
        {
            // cout <<"right " << right <<' '<<left << endl;
            custom_insert(v, sorted, 
                std::min((1<<k) - 1, static_cast<int>(sorted.size())), 
                pend[right], comp);
            right--;
        }
        left = jacobsthal(k);
    }
    if(nchunks & 1)
    {
        custom_insert(v, sorted, sorted.size(), nchunks * chsize -1, comp);
    }
}

int merge_insert_sort(std::vector<unsigned int> &v)
{
    std::vector<unsigned int> sorted;
    std::vector<unsigned int> pend;
    int comp = 0;
    sorted.reserve(v.size());
    pend.resize(v.size());
    sort(v, 1, sorted, pend, comp);
    // cout << v << sorted;
    assert (v.size() == sorted.size());
    for (size_t i = 0; i < sorted.size();i++)
        sorted[i] = v[sorted[i]];
    for (size_t i = 0; i < sorted.size();i++)
        v[i] = sorted[i];
    return comp;
}




static void swap_chunk(int chsize, int base, std::deque<unsigned int> &v)
{
    for (int i = 0; i < chsize; i++)
    {
        std::swap(v[base + i], v[base+i+chsize]);
    }
}


static void custom_insert(
    std::deque<unsigned int> &v,
    std::deque<unsigned int> &sorted,
    size_t pref_size,
    size_t index,
    int &comp
)
{
    unsigned int value = v[index];
    std::deque<unsigned int> sorted_values = sorted;
    for (size_t i = 0; i < sorted_values.size(); i++)
        sorted_values[i] = v[sorted_values[i]];
    // cout << "custom_insert size = " << pref_size << " value = " << value;
    // cout <<  " sorted_values = " << sorted_values;
    size_t l = 0;
    size_t r = pref_size;
    while(l < r)
    {
        int mid = (l+r)/2;
        if(value <= v[sorted[mid]])
        {
            r = mid;
        }
        else 
        {
            l = mid + 1;
        }
        comp++;
    }

    sorted.insert(sorted.begin()+l, index);
    // return l;
}

static void sort(
    std::deque<unsigned int> &v, 
    size_t chsize, 
    std::deque<unsigned int> &sorted,
    std::deque<unsigned int> &pend,
    int &comp
)
{
    size_t dchunk = chsize * 2;
    if (dchunk > v.size()) // single chunk, nothing to do
    {
        sorted.push_back(chsize - 1);
        return;
    }
    // Swap adjacent elements if they are unsorted
    // (since we want to keep the ordering in the recursion, swap the whole chunks)
    for (size_t i = 0; i + dchunk <= v.size(); i += dchunk)
    {
        int h1 = i + chsize - 1;
        int h2 = i + dchunk - 1;
        if(v[h1] > v[h2])
        {
            swap_chunk(chsize, i, v);
        }
        comp++;
    }
    // cout << v << endl;
    sort(v, chsize*2, sorted, pend, comp); // recursive part
    // now we expect that the double chunks are sorted
    // cout << "sorted = " << sorted;
    int nchunks = v.size() / chsize;

    // sorted now represents the positions of the a elements
    sorted.insert(sorted.begin(), sorted[0] ^ chsize);
    int pend_size = 0;
    for (size_t i = 2;i < sorted.size();i++)
    {
        pend[pend_size] = sorted[i] ^ chsize;
        pend_size++;
    }


    // pend.resize(pend_size);
    // cout << "sorted = " << sorted << "pend = " << pend;

    ssize_t left = -1;
    ssize_t right = 0;
    for (int k = 2; left < pend_size; k++)
    {
        right = std::min<ssize_t>(jacobsthal(k), pend_size - 1);
        // cout << k <<' '<<left <<' '<< right << ' ' <<pend_size << endl;
        while(right > left)
        {
            // cout <<"right " << right <<' '<<left << endl;
            custom_insert(v, sorted, 
                std::min((1<<k) - 1, static_cast<int>(sorted.size())), 
                pend[right], comp);
            right--;
        }
        left = jacobsthal(k);
    }
    if(nchunks & 1)
    {
        custom_insert(v, sorted, sorted.size(), nchunks * chsize -1, comp);
    }
}

int merge_insert_sort(std::deque<unsigned int> &v)
{
    std::deque<unsigned int> sorted;
    std::deque<unsigned int> pend;
    int comp = 0;
    pend.resize(v.size());
    sort(v, 1, sorted, pend, comp);
    // cout << v << sorted;
    assert (v.size() == sorted.size());
    for (size_t i = 0; i < sorted.size();i++)
        sorted[i] = v[sorted[i]];
    for (size_t i = 0; i < sorted.size();i++)
        v[i] = sorted[i];
    // cout << "result = " << v << endl;
    return comp;
}
