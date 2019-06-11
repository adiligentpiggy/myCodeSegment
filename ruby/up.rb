#!/usr/local/luna-ruby/bin/ruby

# multipart/form-data
require 'uri'
require 'net/http'
require 'json'
url = URI('http://127.0.0.1/v2/audio/upload_file.json')
http = Net::HTTP.new(url.host, url.port)

data = [
    ['name', '45'],
    ['medium[virtual_file][]', open('45.mp3'), { filename: '45.mp3'}]
]
request = Net::HTTP::Post.new(url.path)
request.set_form(data, 'multipart/form-data')
response = http.request(request)
puts response.body
