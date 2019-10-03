#ifndef ADRIVAL_SERIES_H
#define ADRIVAL_SERIES_H


namespace adrival {

	inline namespace series {
		auto AP = [](auto init, auto step) {
			auto cur = init;
			return [=]() mutable {
				cur += step;
				return cur - step;
			};
		};

		auto GP = [](auto init, auto step) {
			auto cur = init;
			return [=]() mutable {
				cur *= step;
				return cur / step;
			};
		};
}

}
#endif // !ADRIVAL_SERIES_H