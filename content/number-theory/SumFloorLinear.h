/**
 * Author: wleung_bvg
 * Description: Computes the sum of floor((a * i + b) / m) for 0 <= i < n
 * Equivalent to the number of integer points (x, y) where 0 <= x < n and
 * 0 <= y <= (a * i + b) / m
 */
template <class T> T sumLinear(T n, T a, T b, T m) {
  if (a == 0) return (b / m) * n;
  if (a >= m || b >= m)
    return ((a / m) * (n - 1) + 2 * (b / m)) * n / 2
        + sumLinear(n, a % m, b % m, m);
  return sumLinear((a * n + b) / m, m, (a * n + b) % m, a);
}
