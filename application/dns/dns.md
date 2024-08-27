
[TOC]

# struction

<table>
	<tr>
		<td>0</td>
		<td>1</td>
		<td>2</td>
		<td>3</td>
		<td>4</td>
		<td>5</td>
		<td>6</td>
		<td>7</td>
		<td>8</td>
		<td>9</td>
		<td>10</td>
		<td>11</td>
		<td>12</td>
		<td>13</td>
		<td>14</td>
		<td>15</td>
		<td>16</td>
		<td>17</td>
		<td>18</td>
		<td>19</td>
		<td>20</td>
		<td>21</td>
		<td>22</td>
		<td>23</td>
		<td>24</td>
		<td>25</td>
		<td>26</td>
		<td>27</td>
		<td>28</td>
		<td>29</td>
		<td>30</td>
		<td>31</td>
		<td>bit</td>
	</tr>
	<tr>
		<td colspan='16'><center>Transation ID</center></td>
		<td colspan='16'><center>Flags</center></td>
	</tr>
	<tr>
		<td colspan='16'><center>Questions</center></td>
		<td colspan='16'><center>Answers RRs</center></td>
	</tr>
	<tr>
		<td colspan='16'><center>Authority RRs</center></td>
		<td colspan='16'><center>Additional RRs</center></td>
	</tr>
	<tr>
		<td colspan='32'><center>Queries</centere></td>
	</tr>
	<tr>
		<td colspan='32'><center>Answers</center><td>
	</tr>
	<tr>
		<td colspan='32'><center>Authoritative nameservers</center><td>
	</tr>
	<tr>
		<td colspan='32'><center>Additional records</center></td>
	</tr>
</table>

*Transation ID* : DNS ID , to identify each packet
*Flags* :
<table>
	<tr>
		<td>QR</td>
		<td>Opcode</td>
		<td>AA</td>
		<td>TC</td>
		<td>RD</td>
		<td>RA</td>
		<td>Z</td>
		<td>Answer authenticated</td>
		<td>Non-authenticated data</td>
		<td>rcode</td>
	</tr>
</table>
- QR (1 bit) : when the packet is a query request , the value is 0 , when it is response packet , the value is 1
- Opcode (4 bits) : 0 represent standard query , 1 represent reverse query , 2 represent server status request
- AA(Authoritative) (1 bit) : Authoritative response , only be effictive in response packet . 1 represent server is a authoritative server , if 0 represent server is not authoritative server 
- TC(truncated) (1 bit) : represent if be truncated . when the value is 1 , represent the response is more then 512 bytes , only return 512 bytes in the front of packet
- RD(Recursion Desired) (1 bit) : when the value is 1 , represent the server should to deal with this query request . the value is 0 represent the server can return a server list which can solve the query request  
- RA(Recursion Availiable) (1 bit) : only effictive in response packet , the value is 1 represent the server can seearch by recursion 
- Z (1 bit) : must be zero
- Answer authenticated (1 bit) : 0
- Non-authenticated data (1 bit) : 0
- rcode(Reply Code) (4 bits) : represent the error status . 0 represent no error , 1 represent format error , 2 represent server failure , 3 represent name error , 4 represent not implement , 5 represent refused 


# DNS packet

## query 
05 e5 01 00 00 01 00 00 00 00 00 00 05 62 61 69 64 75 03 63 6f 6d 00 00 01 00 01

Transation ID (2 bytes) : 05 e5
Flag :
- QR (1 bit) : 0
- Opcode (4 bits) : 0000
- AA (1 bit) : None
- TC (1 bit) : 0
- RD (1 bit) : 1
- RA (1 bit) : None
- Z (1 bit) : 0
- Answers authenticated (1 bit) : None
- Non-authenticated (1 bit) : None
- rcode (4 bits) : None 

Questions (2 bytes) : 00 01
Answers RRs (2 bytes) : 00 00 
Authority RRs (2 bytes) :00 00
Additional RRs (2 bytes) :00 00
Queries (4 bytes) :05 62 61 69 64 75 03 63 6f 6d 00 00 01 00 01 
- name () : 05 62 61 64 75 03 63 6f 00
- type : 00 01  
- class : 00 01
Answers (4 bytes) : None
Authoritative nameservers (4 bytes) : None
Additional records (4 bytes): None

## response 

05 e5 81 80 00 01 00 02 00 00 00 00 05 62 61 69 64 75 03 63 6f 6d 00 00 01 00 01 c0 0c 00 01 00 01 00 00 01 05 00 04 dc b5 26 94 c0 0c 00 01 00 01 00 00 01 05 00 04 dc b5 26 fb

Transation ID (2 bytes) : 05 e5
Flags : 81 80
- QR (1 bit) : 1
- Opcode (4 bits) : 00 00
- AA (1 bit) : 0 
- TC (1 bit) : 0
- RD (1 bit) : 1
- RA (1 bit) : 1
- Z (1 bit) : 1
- Answers authenticated (1 bit) : 0
- Non-authenticated (1 bit) : 0
- rcode (4 bits) : 00 00

Questions (2 bytes) : 00 01
Answer RRs (2 bytes) : 00 02
Authority RRs (2 bytes) : 00 00
Additional RRs (2 bytes) : 00 00
Queries
- name () : 05 62 61 69 64 75 03 63 6f 00 00 01 00 01
- Type : 00 01
- Class : 00 01

Answers
- name (2 bytes) : c0 0c
- Type (2 bytes) : 00 01
- Class (2 bytes) :00 01
- Time to live (4 bytes) : 00 00 01 05
- Data length (1 byte) : 04
- Address (4 bytes) : dc b5 26 94 

Answers



