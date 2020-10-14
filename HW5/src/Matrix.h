#ifndef OTUSHOMEWORKS_MATRIX_H
#define OTUSHOMEWORKS_MATRIX_H
#include <vector>
#include <map>
#include <array>
#include <memory>

template <typename T, T t_value, size_t dimensional>
struct Matrix_Infinity_Iterator;

template <typename T, T t_value>
struct Matrix_Node{
private:
    std::map<size_t, std::pair<T, std::vector<size_t>>> mat;
    std::vector<size_t> positions;
    typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator zombie = mat.end();

    void kill_zombie() {
        if (zombie != mat.end() && zombie->second.first == t_value)
            mat.erase(zombie);
        zombie = mat.end();
    }

public:

    void SetPosition(std::vector<size_t> pos) {
        positions = pos;
    }

    T& operator[](size_t i) {
        kill_zombie();
        zombie = mat.find(i);
        if (zombie == mat.end()) {
            zombie = mat.emplace(i, std::pair{t_value, positions}).first;
        }
        return zombie->second.first;
    }

    void deep_begin(std::vector<typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator> & vec){
        kill_zombie();
        vec.push_back(begin());
    }

    void deep_end(std::vector<typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator> & vec){
        kill_zombie();
        vec.push_back(end());
    }

    auto begin()
    {
        kill_zombie();
        return mat.begin();
    }

    auto end()
    {
        kill_zombie();
        return mat.end();
    }

    size_t size() {
        kill_zombie();
        return mat.size();
    }
};

template <typename T, T t_value, size_t dimensional = 2>
struct Matrix {
private:
    std::map<size_t, std::shared_ptr<Matrix<T, t_value, dimensional-1>>> mat;
    std::vector<size_t> positions;
public:

    void SetPosition(std::vector<size_t> pos) {
        positions = pos;
    }

    auto & operator[](size_t i) {
        if (mat.find(i) == mat.end()){
            auto it = mat.emplace(i, std::make_shared<Matrix<T, t_value, dimensional - 1>>());
            positions.push_back(it.first->first);
            it.first->second->SetPosition(positions);
            positions.pop_back();
        }
        return *mat[i];
    }

    void deep_begin(std::vector<typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator> & vec){
        for (auto & i : mat){
            i.second->deep_begin(vec);
        }
    }

    void deep_end(std::vector<typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator> & vec){
        for (auto & i : mat){
            i.second->deep_end(vec);
        }
    }

    typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator GetEnd(){
        return prev(mat.end())->second->GetEnd();
    }

    auto begin()
    {
        return Matrix_Infinity_Iterator<T, t_value, dimensional>{*this, 0};
    }

    auto end()
    {
        return Matrix_Infinity_Iterator<T, t_value, dimensional>{this->GetEnd()};
    }

    size_t size() {
        size_t result = 0;
        size_t size = 0;
        for (auto it = mat.begin(); it != mat.end();) {
            size = it->second->size();
            result += size;
            if (size == 0) {
                it = mat.erase(it);
            } else {
                it++;
            }
        }
        return result;
    }
};


template <typename T, T t_value>
struct Matrix<T, t_value, 2> {
private:
    std::map<size_t, std::shared_ptr<Matrix_Node<T, t_value>>> mat;
    std::vector<size_t> positions;
public:

    void SetPosition(std::vector<size_t> pos) {
        positions = pos;
    }

    auto & operator[](size_t i) {
        if (mat.find(i) == mat.end()){
            auto it = mat.emplace(i, std::make_shared<Matrix_Node<T, t_value>>());
            positions.push_back(it.first->first);
            it.first->second->SetPosition(positions);
            positions.pop_back();
        }
        return *mat[i];
    }

    void deep_begin(std::vector<typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator> & vec){
        for (auto & i : mat){
            i.second->deep_begin(vec);
        }
    }

    void deep_end(std::vector<typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator> & vec){
        for (auto & i : mat){
            i.second->deep_end(vec);
        }
    }

    typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator GetEnd(){
        return prev(mat.end())->second->end();
    }

    auto begin()
    {
        return Matrix_Infinity_Iterator<T, t_value, 2>{*this, 0};
    }

    auto end()
    {
        return Matrix_Infinity_Iterator<T, t_value, 2>{this->GetEnd()};
    }

    size_t size() {
        size_t result = 0;
        size_t size = 0;
        for (auto it = mat.begin(); it != mat.end();) {
            size = it->second->size();
            result += size;
            if (size == 0) {
                it = mat.erase(it);
            } else {
                it++;
            }
        }
        return result;
    }
};

template <typename T, std::size_t... Indices, typename U>
auto vectorToTupleImpl(const std::vector<T>& v, std::index_sequence<Indices...>, U val) {
    return std::make_tuple(v[Indices]..., val);
}

template <std::size_t N, typename T, typename U>
auto vectorToTuple(const std::vector<T>& v, U value) {
    assert(v.size() >= N);
    return vectorToTupleImpl(v, std::make_index_sequence<N>(), value);
}

template <typename T, T t_value, size_t dimensional>
struct Matrix_Infinity_Iterator{
private:
    typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator val;
    size_t i;
    std::vector<typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator> begins;
    std::vector<typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator> ends;
    std::vector<size_t> keys;
public:
    Matrix_Infinity_Iterator(Matrix<T, t_value, dimensional> & mat,
                             size_t i) : i (i){
        mat.deep_begin(begins);
        mat.deep_end(ends);
        val = begins[i];
    }

    Matrix_Infinity_Iterator(typename std::map<size_t, std::pair<T, std::vector<size_t>>>::iterator mat){
        val = mat;
    }

    bool operator!=(const Matrix_Infinity_Iterator<T, t_value, dimensional>& it)
    {
        return val != it.val;
    }

    void operator++()
    {
        if (++val == ends[i] && val != ends[ends.size()-1]){
            val = begins[++i];
        }
    }

    auto operator*()
    {
        keys = val->second.second;
        keys.push_back(val->first);
        auto t = vectorToTuple<dimensional, size_t, T>(keys, val->second.first);
        keys.pop_back();
        return t;
    }
};

#endif //OTUSHOMEWORKS_MATRIX_H
