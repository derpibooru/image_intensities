require_relative 'lib/image_intensities/version'

Gem::Specification.new do |s|
  s.name          = 'image_intensities'
  s.version       = ImageIntensities::VERSION
  s.summary       = ''
  s.author        = 'Liam P. White'
  s.files         = `git ls-files`.split("\n")
  s.license       = 'BSD'
  s.extensions    = %w[ext/Rakefile]
  s.require_paths = %w[lib]

  s.add_dependency 'ffi'
  s.add_dependency 'rake'
  s.add_dependency 'rb-libmagic'

end
