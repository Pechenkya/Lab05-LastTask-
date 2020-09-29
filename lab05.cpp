#include <iostream>
#include <iomanip>
#include <vector>
#include <fstream>
#include <cstdlib>

size_t okol_s{};
size_t n{};
size_t m{};
std::vector<int> temp_v;
size_t vec_size{0};

template <typename T, typename Container>
size_t select(Container& container, size_t left, size_t right, size_t n);
template <typename T, typename Container>
size_t partition(Container& container, size_t left_border, size_t right_border, size_t pivot_index, size_t n);
template <typename T, typename Container>
size_t pivot(Container& container, size_t left, size_t right);
template <typename T, typename Container>
size_t partition5(Container& container, size_t left, size_t right);

template <typename T, typename Container>
size_t select(Container& container, size_t left, size_t right, size_t n)
{
	size_t pivot_index{};
	while (true)
	{
		if (left == right)
			return left;
		pivot_index = pivot<T, Container>(container, left, right);
		pivot_index = partition<T, Container>(container, left, right, pivot_index, n);

		if (n == pivot_index)
			return n;
		else if (n < pivot_index)
			right = pivot_index - 1;
		else
			left = pivot_index + 1;
	}
}

template <typename T, typename Container>
size_t partition(Container& container, size_t left_border, size_t right_border, size_t pivot_index, size_t n)
{
	size_t copy_counter{ left_border };
	T pivot = container[pivot_index];

	while (copy_counter <= right_border)
	{
		T comparing_element = container[copy_counter];
		if (comparing_element < pivot)
		{
			std::swap(container[left_border], container[copy_counter]);
			copy_counter++;
			left_border++;
		}
		else if (comparing_element > pivot)
		{
			std::swap(container[copy_counter], container[right_border]);
			right_border--;
		}
		else
		{
			copy_counter++;
		}
	}

	if (n < left_border)
		return left_border;
	else if (n <= right_border)
		return n;
	else
		return right_border;
}

template <typename T, typename Container>
size_t pivot(Container& container, size_t left, size_t right)
{
	if (right - left < 5)
		return partition5<T, Container>(container, left, right);

	for (size_t i = left, sub_right{ i + 4 }, median5{}; i <= right; ++i)
	{
		if (sub_right > right)
			sub_right = right;
		median5 = partition5<T, Container>(container, i, sub_right);
		std::swap(container[median5], container[left + (right - left) / 5]);
	}

	size_t mid = (right - left) / 10 + left + 1;

	return select<T, Container>(container, left, left + (right - left) / 5, mid);
}

template <typename T, typename Container>
size_t partition5(Container& container, size_t left, size_t right)
{
	for (size_t id{ left }; id <= (right + left) / 2; id++)
	{
		size_t min_id = id;
		for (size_t idf{ id + 1 }; idf <= right; idf++)
		{
			if (container[idf] < container[min_id])
				min_id = idf;
		}

		if (min_id != id)
		{
			std::swap(container[min_id], container[id]);
		}
	}

	return (right + left) / 2;
}

int clamp_to_min(int a, int min)
{
	return (a < min) ? min : a;
}

int clamp_to_max(int a, int max)
{
	return (a > max) ? max : a;
}

short get_median()
{
	const size_t m = select<int, std::vector<int>>(temp_v, 0, vec_size - 1, vec_size / 2);
	if (vec_size % 2 == 1)
	{
		vec_size = 0;
		return temp_v[m];
	}
	else
	{
		vec_size = 0;
		return (temp_v[m] + temp_v[m + 1]) / 2;
	}
}

void set_median(short int** matrix, short int** result_matrix, int i, int j)
{
	for (size_t c = clamp_to_min(i - (int)okol_s / 2, 0),
		endc = clamp_to_max(i + (int)okol_s / 2, n - 1);
			c <= endc; ++c)
	{
		for (size_t r = clamp_to_min(j - (int)okol_s / 2, 0),
			endr = clamp_to_max((j + (int)okol_s / 2), m - 1);
				r <= endr; ++r)
		{
			temp_v.at(vec_size++) = matrix[c][r];
		}
	}
	result_matrix[i][j] = get_median();
}

int main()
{
	//Creating arrays, reading data
	short int** matrix; 
	short int** result_matrix;
	std::ifstream in("matrix_input.txt");
	std::ofstream out("matrix_result.txt");
	in >> okol_s;
	temp_v.resize(okol_s * okol_s + 1);
	in >> n >> m;

	matrix = new short int*[n];
	result_matrix = new short int*[n];
	for (size_t i = 0; i < n; i++)
	{
		matrix[i] = new short int[m];
		result_matrix[i] = new short int[m];
		for (size_t j = 0; j < m; j++)
			in >> matrix[i][j]/* = rand() % 255 + 1*/;
	}
	//

	std::cout << "Matrix before:\n\n";
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
			std::cout << std::setw(3) << matrix[i][j] << ' ';
		std::cout << std::endl;
	}

	//Main calculations
	for (size_t i = 0; i < n; i++)
		for (size_t j = 0; j < m; j++)
			set_median(matrix, result_matrix, i, j);
	//

	std::cout << "\n\nMatrix after:\n\n";
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < m; j++)
		{
			std::cout << std::setw(3) << result_matrix[i][j] << ' ';
			out << std::setw(3) << result_matrix[i][j] << ' ';
		}
		std::cout << std::endl;
		out << std::endl;
	}

	//Cleating data, closing streams
	for (size_t i = 0; i < n; i++)
		delete[] matrix[i];
	delete[] matrix;
	in.close();
	out.close();
	//
	system("Pause");
	return 0;
}

