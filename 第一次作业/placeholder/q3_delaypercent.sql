select distinct Shipper.CompanyName,Res.DelayPercentage
from Shipper,(select LateOrder.ShipVia,LateOrder.LateOrders,AllOrder.AllOrders,round((LateOrder.LateOrders*1.0/AllOrder.AllOrders)*100,2) as DelayPercentage
		     from (select ShipVia,count(Id) as LateOrders
			  from 'Order'
			  where ShippedDate>RequiredDate
			  group by ShipVia) as LateOrder,
			  (select ShipVia,count(Id) as AllOrders
			  from 'Order'
			  group by ShipVia) as AllOrder	
		     where LateOrder.ShipVia=ALlOrder.ShipVia) as Res
where Res.ShipVia=Shipper.Id
order by Res.DelayPercentage desc
;
