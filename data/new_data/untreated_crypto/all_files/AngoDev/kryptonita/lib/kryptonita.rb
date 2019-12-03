require "kryptonita/version"
#require "./whirlpool/whirlpool"
require "digest"

# Support multiple ruby versions, fat binaries under Windows.
begin
  RUBY_VERSION =~ /(\d+.\d+)/
  require "whirlpool/#{$1}/whirlpool"
rescue LoadError
  require 'whirlpool/whirlpool'
end

module Kryptonita
  class Hash

    def self.md5(str, salt: nil)
      if salt.nil?
        Digest::MD5.hexdigest(str)
      else
        Digest::MD5.hexdigest(Digest::MD5.digest(str) + salt)
      end
    end

    def self.sha1(str)
      Digest::SHA1.hexdigest(str)
    end

    def self.sha512(str)
      Digest::SHA512.hexdigest(str)
    end

    def self.whirlpool(str)
      w = Whirlpool::Class.new
      w.print_string(str).downcase
    end

  end
end
