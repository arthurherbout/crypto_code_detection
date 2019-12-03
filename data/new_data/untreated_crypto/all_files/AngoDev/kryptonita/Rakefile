require "bundler/gem_tasks"
require "rspec/core/rake_task"
require 'rake/extensiontask'

RSpec::Core::RakeTask.new(:spec)

gemspec = Gem::Specification::load(File.expand_path('../kryptonita.gemspec', __FILE__))

task :build => [:clean, :compile]

task default: [:build, :spec]

task :console do
  exec "irb -r kryptonita -I ./lib"
end

Rake::ExtensionTask.new("whirlpool", gemspec) do |ext|
  ext.lib_dir = "lib/whirlpool"
  if RUBY_PLATFORM =~ /mswin|mingw/ then
    # Define target for extension (Supporting fat binaries)
    RUBY_VERSION =~ /(\d+\.\d+)/
    ext.lib_dir = "lib/whirlpool/#{$1}"
  else
    ext.cross_compile = true
    ext.cross_platform = []
    ext.cross_config_options << "--disable-lookup"
    config_opts = {}
    platform_host_map = {
      'x86-mingw32' => 'i586-mingw32msvc',
      'x64-mingw32' => 'x86_64-w64-mingw32'
    }
  end
end
