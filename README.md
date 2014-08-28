frequency_meter
===============

Code for msp430g2553 to measure the frequency of the electrical signal.

Limitation: >10Hz

Terminal configuration: 1200 baud rate, 8N1

Protocol: 'a' to Request


                                       Vcc                            Vcc                            
      +------------+                    +                              +                             
      |            |                    |                      +-------+                             
      |           +++                  +++                     |                                     
      |           | |180Kohm           | |100Kohm              |                                     
      |           +++                  +++                     |                                     
      +            |                    |                      |                                     
127rms 60Hz        |        +------+    |                    +-+--+                 +--------+
      +            +--------+      +----+----+               |    |RX---------------+        |
      |                     |Sfh620A         +----------+P2.0|    +TX---------------+ PC     |
      |            +--------+      +----+                    |Msp430g2553           |        |
      |            |        +------+    |                    |    +                 |        |
      |            |                    |                    |    +---+             |        |
      +------------+                    |                    +----+   |             |        |
                                        |                             |             +--------+
                                        |                             |                              
                                        |                             |                              
                                      ++-++                           |                              
                                       +-+                          ++-++                            
                                        +                            +-+                             
                                                                      +                              
