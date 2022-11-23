/**
 * Author: wleung_bvg
 * Description: Binary search over a range for the first or last occurrence of a return value for a boolean function.
 * If ISFIRST is true, returns the first value in the range [lo, hi) where f(x) is true.
 * IF ISFIRST is false, returns the last value in the range [lo, hi) where f(x) is true.
 */
const bool FIRST = true, LAST = false;
template <const bool ISFIRST, class T, class F>
T bsearch(T lo, T hi, F f) {
  hi--; while (lo <= hi) {
    T mid = lo + (hi - lo) / 2;
    if (f(mid) == ISFIRST) hi = mid - 1;
    else lo = mid + 1;
  }
  return ISFIRST ? lo : hi;
}
