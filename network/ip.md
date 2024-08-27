
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
		<td colspan='4'><center>version</center></td>
		<td colspan='4'><center>IHL</center></td>
		<td colspan='8'><center>tos</center></td>
		<td colspan='16'><center>totlen</center></td>
	</tr>
	<tr>
		<td colspan='16'><center>identification</center></td>
		<td colspan='3'><center>flag</center></td>
		<td colspan='13'><center>offsetfrag</center></td>
	</tr>
	<tr>
		<td colspan='8'><center>TTL</center></td>
		<td colspan='8'><center>protocol</center></td>
		<td colspan='16'><center>checksum</center></td>
	</tr>
	<tr>
		<td colspan='32'><center>src addr</center></td>
	</tr>
		<td colspan='32'><center>dst addr</center></td>
	</tr>
	<tr>
		<td colspan='24'><center>Optional</centere></td>
		<td colspan='8'><center>padding</center></td>
	</tr>
	<tr>
		<td colspan='32'><center>data</center></td>
	</tr>
</table>

*version* (4 bits) : IPv4 is 4 , Ipv6 is 6 
*IHL* (4 bits) : length of header
*tos* (8 bits) : service type
*totlen* (16 bits) : the length of header and data
*identification* (16 bits) : identification 
*flag* (3 bits) : the first bit is 0 . the second bit is DF , the value is 0 , the packet is fragment , the value is 0 , the packet is no fragment . the third bit is 0 represent there is no data fragment is on transmition 
*offsetfrag* (13 bits) : if the packet is fragment , offsetfrag represent the offset of the whole packet
*TTL* (8 bits) : 
*protocol* (8 bits) : 
<table>
	<tr>
		<td>6</td>
		<td>17</td>
		<td>1</td>
	</tr>
	<tr>
		<td>TCP</td>
		<td>UDP</td>
		<td>ICMP</td>
	</tr>
</table>
*checksum* (16 bits) : 
*src addr* (32 bits) :
*dst addr* (32 bits) :
*Optional* :
*padding* : to make sure IHL%4 equel to 0



