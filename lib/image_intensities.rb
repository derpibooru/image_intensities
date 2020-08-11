# frozen_string_literal: true

require 'magic'

module ImageIntensities
  def self.file(path)
    mime = magic.file(path)
    case mime
    when 'image/png'
      ins = Native.png_intensities(path)
      raise 'Processing error' if ins[:error] != 0

      { nw: ins[:nw], ne: ins[:ne], sw: ins[:sw], se: ins[:se] }
    when 'image/jpeg'
      ins = Native.jpeg_intensities(path)
      raise 'Processing error' if ins[:error] != 0

      { nw: ins[:nw], ne: ins[:ne], sw: ins[:sw], se: ins[:se] }
    else
      raise "Unsupported file type `#{mime}'"
    end
  end

  def self.magic
    @magic ||= Magic.new(Magic::MIME_TYPE)
  end
end

require 'image_intensities/native'
require 'image_intensities/version'
