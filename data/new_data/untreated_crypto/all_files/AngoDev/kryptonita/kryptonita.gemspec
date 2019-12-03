# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'kryptonita/version'

Gem::Specification.new do |spec|
  spec.name          = "kryptonita"
  spec.version       = Kryptonita::VERSION
  spec.platform = Gem::Platform::RUBY
  spec.authors       = ["Patrício dos Santos"]
  spec.email         = ["ps@patriciodossantos.net"]
  spec.summary       = %q{Kryptonita is a Ruby gem that provides a lot of functions for hashing, encrypt and decrypt}
  spec.description   = %q{Kryptonita is a Ruby gem that provides a lot of functions for hashing, encrypt and decrypt.}
  spec.homepage      = "https://github.com/AngoDev/kryptonita"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0")
  spec.executables   = spec.files.grep(%r{^bin/}) { |f| File.basename(f) }
  spec.test_files    = spec.files.grep(%r{^(test|spec|features)/})
  spec.require_paths = ["lib"]

  spec.require_paths = ['lib']
  spec.extensions = %w[ext/whirlpool/extconf.rb]

  spec.required_ruby_version = ">= 2.0.0"
  spec.add_development_dependency "bundler", "~> 1.7"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rspec", "~> 3.1.0"
  spec.add_development_dependency "rake-compiler"
end
