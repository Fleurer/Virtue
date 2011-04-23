require 'test/unit'
require 'socket'

class TestVirtue < Test::Unit::TestCase
  def test_run
    run_socket('bin/vt', '127.0.0.1', 8888) do |s|
      s.send('hello world',0)
      s.shutdown(Socket::SHUT_WR)
      puts s.recv(512)
    end
  end

  def run_socket(path, host, port)
    pid = fork do
      exec path
    end
    trap('INT'){ kill(9, pid) }
    sleep 0.2
    TCPSocket::open(host, port) do |s|
      yield s
    end
  end
end
