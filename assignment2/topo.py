from mininet.cli import CLI
from mininet.net import Mininet
from mininet.link import TCLink
from mininet.topo import Topo
from mininet.log import setLogLevel

class StarFishTopo(Topo):
    def __init__(self, **opts):
        Topo.__init__(self, **opts)
        h1 = self.addHost('h1')
        h2 = self.addHost('h2')
        h3 = self.addHost('h3')
        h4 = self.addHost('h4')

        s1 = self.addSwitch('s1')

        self.addLink(h1, s1)
        self.addLink(h2, s1)
        self.addLink(h3, s1)
        self.addLink(h4, s1)

if __name__ == '__main__':
    setLogLevel( 'info' )

    # Create data network
    topo = StarFishTopo()
    net = Mininet(topo=topo, link=TCLink, autoSetMacs=True,
           autoStaticArp=True)

    # Start programs
    h1 = net.get('h1')
    h1.cmd("pyhon starter_files/start_server.py 1")

    h2 = net.get('h2')
    h2.cmd("go run printer.go &")

    h3 = net.get('h3')
    h3.cmd("pyhon starter_files/launch_firefox.py 1 & ")

    # Run network
    net.start()
    CLI(net)
    net.stop()

