#pragma once

#ifndef ZIMG_GRAPH_GRAPHBUILDER_H_
#define ZIMG_GRAPH_GRAPHBUILDER_H_

#include <memory>
#include <vector>
#include "common/pixel.h"
#include "colorspace/colorspace.h"
#include "depth/depth.h"
#include "resize/resize.h"
#include "unresize/unresize.h"

namespace zimg {

enum class CPUClass;

namespace graph {

class FilterGraph;
class ImageFilter;

/**
 * Factory interface for filter instantiation.
 */
class FilterFactory {
public:
	typedef std::vector<std::unique_ptr<ImageFilter>> filter_list;

	/**
	 * Destroy factory.
	 */
	virtual ~FilterFactory() = 0;

	/**
	 * Create filters implementing colorspace conversion.
	 *
	 * @param conv conversion specifier
	 * @return list of filters
	 */
	virtual filter_list create_colorspace(const colorspace::ColorspaceConversion &conv) = 0;

	/**
	 * Create filters implementing depth conversion.
	 *
	 * @see create_colorspace
	 */
	virtual filter_list create_depth(const depth::DepthConversion &conv) = 0;

	/**
	 * Create filters implementing resizing.
	 *
	 * @see create_colorspace
	 */
	virtual filter_list create_resize(const resize::ResizeConversion &conv) = 0;

	/**
	 * Create filters implementing unresizing.
	 *
	 * @see create_unresize
	 */
	virtual filter_list create_unresize(const unresize::UnresizeConversion &conv) = 0;
};

/**
 * Default implementation of factory interface.
 */
class DefaultFilterFactory : public FilterFactory {
public:
	filter_list create_colorspace(const colorspace::ColorspaceConversion &conv) override;

	filter_list create_depth(const depth::DepthConversion &conv) override;

	filter_list create_resize(const resize::ResizeConversion &conv) override;

	filter_list create_unresize(const unresize::UnresizeConversion &conv) override;
};


/**
 * Manages initialization of filter graphs.
 */
class GraphBuilder {
public:
	enum class ColorFamily {
		GREY,
		RGB,
		YUV,
	};

	enum class FieldParity {
		PROGRESSIVE,
		TOP,
		BOTTOM,
	};

	enum class ChromaLocationW {
		LEFT,
		CENTER,
	};

	enum class ChromaLocationH {
		CENTER,
		TOP,
		BOTTOM,
	};

	/**
	 * Filter instantiation parameters.
	 */
	struct params {
		std::unique_ptr<const resize::Filter> filter;
		std::unique_ptr<const resize::Filter> filter_uv;
		bool unresize;
		depth::DitherType dither_type;
		double peak_luminance;
		bool approximate_gamma;
		bool scene_referred;
		CPUClass cpu;

		params() noexcept;
	};

	/**
	 * Image format specifier.
	 */
	struct state {
		unsigned width;
		unsigned height;
		PixelType type;
		unsigned subsample_w;
		unsigned subsample_h;

		ColorFamily color;
		colorspace::ColorspaceDefinition colorspace;

		unsigned depth;
		bool fullrange;

		FieldParity parity;
		ChromaLocationW chroma_location_w;
		ChromaLocationH chroma_location_h;

		double active_left;
		double active_top;
		double active_width;
		double active_height;
	};
private:
	struct resize_spec;

	std::unique_ptr<FilterGraph> m_graph;
	state m_state;

	void attach_filter(std::shared_ptr<ImageFilter> filter);

	void attach_filter_uv(std::shared_ptr<ImageFilter> filter);

	void color_to_grey(colorspace::MatrixCoefficients matrix);

	void grey_to_color(ColorFamily color, colorspace::MatrixCoefficients matrix, unsigned subsample_w, unsigned subsample_h,
	                   ChromaLocationW chroma_location_w, ChromaLocationH chroma_location_h);

	void convert_colorspace(const colorspace::ColorspaceDefinition &colorspace, const params *params, FilterFactory *factory);

	void convert_depth(const PixelFormat &format, const params *params, FilterFactory *factory);

	void convert_resize(const resize_spec &spec, const params *params, FilterFactory *factory);
public:
	/**
	 * Default construct GraphBuilder, creating a builder that manages no graph.
	 */
	GraphBuilder() noexcept;

	/**
	 * Destroy builder.
	 */
	~GraphBuilder();

	/**
	 * Set image format of graph input. Creates a new graph.
	 *
	 * @param source image format
	 * @return reference to self
	 */
	GraphBuilder &set_source(const state &source);

	/**
	 * Connect graph to target image format.
	 *
	 * @param target image format
	 * @param params filter creation parameters
	 * @return reference to self
	 */
	GraphBuilder &connect_graph(const state &target, const params *params, FilterFactory *factory = nullptr);

	/**
	 * Finalize and return managed graph.
	 *
	 * @return graph
	 */
	std::unique_ptr<FilterGraph> complete_graph();
};

} // namespace graph
} // namespace zimg

#endif // ZIMG_GRAPH_GRAPHBUILDER_H_
