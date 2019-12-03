require "spec_helper"

module Kryptonita
  describe Hash do

    describe "md5" do
      let(:hashed) { Kryptonita::Hash.md5("ruby") }
      let(:hashed_with_salt) { Kryptonita::Hash.md5("ruby", salt: 'salt') }

      it "returns a string" do
        expect(hashed).to be_a(String)
        expect(hashed_with_salt).to be_a(String)
      end

      it "returns a string with size 32" do
        expect(hashed.size).to eq(32)
        expect(hashed_with_salt.size).to eq(32)
      end

      it "returns a correct hash" do
        expect(hashed).to eql("58e53d1324eef6265fdb97b08ed9aadf")
        expect(hashed_with_salt).to eql("cb2dfb8002036f039bd61a1c1fbbbd3c")
      end
    end

    describe "sha1" do
      let(:hashed) { Kryptonita::Hash.sha1("ruby") }

      it "returns a string" do
        expect(hashed).to be_a(String)
      end

      it "returns a string with a size 40" do
        expect(hashed.size).to eq(40)
      end

      it "returns a correct hash" do
        expect(hashed).to eql("18e40e1401eef67e1ae69efab09afb71f87ffb81")
      end
    end

    describe "#sha512" do
      let(:hashed) { Kryptonita::Hash.sha512("ruby") }

      it "returns a string" do
        expect(hashed).to be_a(String)
      end

      it "returns a string with a size 128" do
        expect(hashed.size).to eq(128)
      end

      it "returns a correct hash" do
        expect(hashed).to eql("423408d7723a3d80baefa804bd50b61a89667efec1713386a7b8efe28e5d13968307a908778cad210d7aa2dfe7db9a2aa86895f9fc1eeefcc99814310b207a6b")
      end
    end

    describe "#whirlpool" do
      let(:hashed) { Kryptonita::Hash.whirlpool("ruby") }

      it "returns a string" do
        expect(hashed).to be_a(String)
      end

      it "returns a string with a size 128" do
        expect(hashed.size).to eq(128)
      end

      it "returns a correct hash" do
        expect(hashed).to eql("95fc6a05b1edd849a202d9cdb1158930cf1e101900357a8816b743520710be2487c890c3bfb2b70f2308f7e8737473a477bb44950516c23e53a2993091faa9d2")
      end
    end


  end
end
