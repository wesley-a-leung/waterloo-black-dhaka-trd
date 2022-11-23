/**
 * Author: wleung_bvg
 * Description: Ternary search for the maximum of a function
 */
template <class T, class F, class Cmp = less<T>>
T tsearch(T lo, T hi, F f, int iters, Cmp cmp = less<T>()) {
  for (int it = 0; it < iters; it++) {
    T m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
    if (cmp(f(m1), f(m2))) lo = m1;
    else hi = m2;
  }
  return lo + (hi - lo) / 2;
}
