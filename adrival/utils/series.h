#ifndef ADRIVAL_SERIES_H
#define ADRIVAL_SERIES_H


namespace adrival {

	inline namespace series {
		auto AP = [](int init, int step) {
			auto cur = init;
			return [=]() mutable {
				cur += step;
				return cur - step;
			};
		};

		auto GP = [](int init, int step) {
			auto cur = init;
			return [=]() mutable {
				cur *= step;
				return cur / step;
			};
		};
}

}
#endif // !ADRIVAL_SERIES_H