# frozen_string_literal: true

require 'ffi'

module ImageIntensities
  module Native
    extend FFI::Library

    spec = Gem.loaded_specs['image_intensities']
    gem_root = spec.gem_dir
    ffi_lib "#{gem_root}/ext/image_intensities/lib/libimage_intensities.so"

    class IntensityData < FFI::Struct
      layout :nw, :double,
             :ne, :double,
             :sw, :double,
             :se, :double,
             :error, :int
    end

    attach_function :jpeg_intensities, [:string], IntensityData.by_value
    attach_function :png_intensities, [:string], IntensityData.by_value
  end
end
