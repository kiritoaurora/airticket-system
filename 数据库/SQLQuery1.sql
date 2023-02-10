--索引
create nonclustered index flight_index on flight(date);
exec sp_helpindex flight;

create nonclustered index ticket_index on flight(date);
exec sp_helpindex ticket;



--视图1，用于显示对应航班已订票的商务舱数
if exists(select * from sysobjects where name = 'soldOutFirstSeatAmount' and type = 'v')
drop view soldOutFirstSeatAmount
go
create view soldOutFirstSeatAmount
  as
  select airnum,date,count(class_level) 商务舱数
  from ticket 
  where class_level = '商务舱' 
  group by airnum,date
go
--测试
select * from soldOutFirstSeatAmount

--视图2，用于显示 对应航班已订票的经济舱数
if exists(select * from sysobjects where name = 'soldOutSecondSeatAmount' and type = 'v')
drop view soldOutSecondSeatAmount
go
create view soldOutSecondSeatAmount
  as
  select airnum,date,count(class_level) 经济舱数
  from ticket
  where class_level = '经济舱'
  group by airnum,date
go
--测试
select * from soldOutSecondSeatAmount

--视图3，用于显示各航班的各舱位的定额
if exists(select * from sysobjects where name = 'amountOfSeat' and type = 'v')
drop view amountOfSeat
go
create view amountOfSeat
  as
  select airnum,date,first_class_seat,second_calss_seat
  from flight,airplane
  where flight.planenum = airplane.planenum
go
--测试
select * from amountOfSeat

--视图4，用于显示各航班各舱位的余票量
if exists(select * from sysobjects where name = 'tickets_left' and type = 'v')
drop view tickets_left
go
create view tickets_left
  as
  select amountOfSeat.airnum,amountOfSeat.date,
        (amountOfSeat.first_class_seat-isnull(soldOutFirstSeatAmount.商务舱数,0)) 商务舱余票,
		(amountOfSeat.second_calss_seat-isnull(soldOutSecondSeatAmount.经济舱数,0)) 经济舱余票
  from (soldOutFirstSeatAmount right join amountOfSeat on soldOutFirstSeatAmount.airnum
       = amountOfSeat.airnum and soldOutFirstSeatAmount.date = amountOfSeat.date) left join 
	   soldOutSecondSeatAmount on amountOfSeat.airnum = soldOutSecondSeatAmount.airnum and
	   amountOfSeat.date = soldOutSecondSeatAmount.date
go
--测试
select * from tickets_left

--视图5，用于显示系统查询功能所需要的航班信息
if exists(select * from sysobjects where name = 'flightInformation' and type = 'v')
drop view flightInformation
go
create view flightInformation
  as
  select f.airnum,f.start_station,f.end_station,f.date,f.start_time,f.end_time,plane_model,
  商务舱余票,经济舱余票,price*3.00 商务舱价格,price*1.00 经济舱价格,comp_name
  from flight f,tickets_left tl,airplane ap
  where f.airnum = tl.airnum and f.date = tl.date and f.planenum = ap.planenum
go
--测试
select * from flightInformation

--视图6，用于显示订单信息
if exists(select * from sysobjects where name = 'orderInformation' and type = 'v')
drop view orderInformation
go
create view orderInformation
  as
  select orderform.ordernum,start_station,end_station,ticket.date,orderform.orderDate,count(*) 购买数量,
  total_money,ticket_status,start_time,username
  from (flight right join ticket on ticket.airnum = flight.airnum 
  and ticket.date = flight.date) right join orderform on ticket.ordernum = orderform.ordernum
  group by orderform.ordernum,start_station,end_station,ticket.date,orderform.orderDate,total_money,ticket_status,start_time,username
go
--测试
select * from orderInformation where username = '18725087325'

update ticket set ticket_status = 0 where ticketnum = '070410003'
select seatnum from ticket where ordernum = '100012'
select * from ticket

--视图7，用于显示机票信息
if exists(select * from sysobjects where name = 'ticketInformation' and type = 'v')
drop view ticketInformation
go
create view ticketInformation
  as
  select ticketnum,ticket.airnum,start_station,end_station,ticket.date,start_time,end_time,
         name,class_level,seatnum,ordernum
  from ticket,passage,flight
  where ticket.airnum = flight.airnum and ticket.date = flight.date and ticket.id = passage.id
go
--测试
select * from ticketInformation where ordernum = '100009'

select passage.id,name,phonenum,class_level,seatnum 
                                from passage,ticket,flight 
                                where passage.id = ticket.id 
                                and ticket.airnum = flight.airnum
                                and ticket.date = flight.date
								and start_station like '%'+ '北京'+'%'
								and end_station like '%' +'上海'+'%'
								and flight.date ='2021-07-04'

--视图8，用于管理员查询航班信息
if exists(select * from sysobjects where name = 'adminFlightInfo' and type = 'v')
drop view adminFlightInfo
go
create view adminFlightInfo
  as
  select f.airnum,f.date,f.start_station,f.end_station,plane_model,f.start_time,f.end_time,comp_name,f.price,
  商务舱余票,经济舱余票,(ISNULL(商务舱数,0)+ISNULL(经济舱数,0)) 售票数
  from (flight f left join soldOutFirstSeatAmount soldF on soldF.airnum = f.airnum and soldF.date = f.date) left join
  soldOutSecondSeatAmount soldS on soldS.airnum = f.airnum and soldS.date = f.date,tickets_left tl,airplane ap
  where f.airnum = tl.airnum and f.date = tl.date and f.planenum = ap.planenum 
go
--测试
select * from adminFlightInfo

--  select airline.start_station,airline.end_station,sum(售票数) 售票总数
--  from adminFlightInfo,airline
--  where airline.start_station = adminFlightInfo.start_station and airline.end_station = adminFlightInfo.end_station
--  group by airline.start_station,airline.end_station


--视图9，用于查询指定日期的各航线销售情况
if exists(select * from sysobjects where name = 'soleAmount' and type = 'v')
drop view soleAmount
go
create view soleAmount
  as  
  select flight.airnum,flight.date,start_station,end_station,comp_name,count(*) 销售数量,orderDate
  from orderform,ticket,flight
  where ticket.ordernum = orderform.ordernum and ticket.airnum = flight.airnum and ticket.date = flight.date
  and ticket.ticket_status != 1
  group by flight.airnum,flight.date,start_station,end_station,comp_name,orderDate
go
--测试
select airnum,date,start_station,end_station,comp_name,sum(销售数量) 销量 
from soleAmount 
where orderDate like '%'+'2021-07'+'%'
group by airnum,date,start_station,end_station,comp_name

select airnum,date,start_station,end_station,comp_name,sum(销售数量) 销量 
from soleAmount 
where orderDate = '2021-06-06'
group by airnum,date,start_station,end_station,comp_name



--存储过程1，根据输入查询航班信息
if exists(select name from sysobjects where name = 'searchFlight' and type = 'p')
drop procedure searchFlight
go
create procedure searchFlight
 @start_station varchar(20),@end_station varchar(20),@date date
 as
 select *
 from flightInformation
 where start_station like '%'+@start_station+'%' and end_station like '%'+@end_station+'%' 
       and date = @date
go
--测试
exec searchFlight '北京','上海','2021-07-04'
go

--存储过程2，订票成功时对passage表插入相应的乘客信息
if exists(select name from sysobjects where name = 'newPassage' and type = 'p')
drop procedure newPassage
go
create procedure newPassage
 @id varchar(20),@name varchar(20),@phonenum varchar(20)
 as
 declare @isexist int
 select @isexist = (select count(*) from passage where id = @id)
 if(@isexist != 1)
 begin
 insert into passage(id,name,phonenum)
 values(@id,@name,@phonenum)
 end
go

--测试
exec newPassage '520421198909071490','高胡','15432451269'
go
select * from passage

----------------------------------------------------------------------------------
--存储过程3，订票成功后根据其航班号查询其航司
--if exists(select name from sysobjects where name = 'searchComp' and type = 'p')
--drop procedure searchComp
--go
--create procedure searchComp
-- @airnum varchar(20)
-- as
-- select comp_name
-- from flight
-- where airnum = @airnum
--go
--测试
----------------------------------------------------------------------------------

--存储过程3，订票成功后向member表插入对应的会员信息
if exists(select name from sysobjects where name = 'newMember' and type = 'p')
drop procedure newMember
go
create procedure newMember
 @id varchar(20),@airnum varchar(20),@date date
 as
 declare @comp_name varchar(20),@isexist int
 select @comp_name = (select comp_name from flight where airnum = @airnum and date = @date)
 select @isexist = (select count(*)from member where id = @id and comp_name = @comp_name)
 if(@isexist != 1)
 begin
 insert into member(id,comp_name,mlevel,mileage)
 values(@id,@comp_name,'普通会员',0)
 end
go
--测试
exec newMember '54246319910507652X','CZ6147','2021-07-10'
go
select * from member


--存储过程4，订票成功后向ticket表中插入相应的机票信息
if exists(select name from sysobjects where name = 'newTicket' and type = 'p')
drop procedure newTicket
go
create procedure newTicket
 @ticketnum varchar(20),@airnum varchar(20),@date date,@class_level varchar(20),
 @id varchar(20),@ordernum varchar(20)
 as
 insert into ticket(ticketnum,airnum,date,class_level,id,ordernum)
 values(@ticketnum,@airnum,@date,@class_level,@id,@ordernum)
go
--测试
exec newTicket '060310002','CZ6147','2021-07-10','经济舱','54246319910507652X','100017'
go
select * from ticket
select * from flightInformation

--存储过程5，插入新的订单信息
if exists(select name from sysobjects where name = 'newOrder' and type = 'p')
drop procedure newOrder
go
create procedure newOrder
 @ordernum varchar(20),@orderDate date,@username varchar(20),@total_money float
 as
 insert into orderform(ordernum,orderDate,username,total_money)
 values(@ordernum,@orderDate,@username,@total_money)
go
--测试
exec newOrder '100016','2021-06-06','18725087325',650
go

--存储过程6，用于实现机票价格的浮动
if exists(select name from sysobjects where name = 'priceFluctuation' and type = 'p')
drop procedure priceFluctuation
go
create procedure priceFluctuation
 as
 update flight
 set price = price * 1.25
 --where DATEDIFF(DAY,@nowDate,date) < 4 and DATEDIFF(DAY,@nowDate,date) > 0
 where DATEDIFF(DAY,GETDATE(),date) < 4 and DATEDIFF(DAY,GETDATE(),date) > 0
go
--测试
exec priceFluctuation
select * from flightInformation
select * from flight
--存储过程7，用于将机票价格基本价格回溯
if exists(select name from sysobjects where name = 'rePriceFluctuation' and type = 'p')
drop procedure rePriceFluctuation
go
create procedure rePriceFluctuation
 as
 update flight
 set price = price / 1.25
 --where DATEDIFF(DAY,@nowDate,date) < 4 and DATEDIFF(DAY,@nowDate,date) > 0
 where DATEDIFF(DAY,GETDATE(),date) < 4 and DATEDIFF(DAY,GETDATE(),date) > 0
go
--测试
exec rePriceFluctuation
select * from flight

--存储过程8，查询账号下已添加过的乘机人信息
if exists(select name from sysobjects where name = 'oldPassagers' and type = 'p')
drop procedure oldPassagers
go
create procedure oldPassagers
 @username varchar(20)
 as
 select distinct passage.id,passage.name,passage.phonenum
 from orderform,ticket,passage
 where.orderform.ordernum = ticket.ordernum and passage.id = ticket.id
  and username = @username
go
--测试
exec oldPassagers '18725087325'


--触发器1,订票成功后为乘客的会员信息增加对应的里程数
if exists(select * from sysobjects where name = 'addMileage' and type = 'tr')
drop trigger addMileage
go
create trigger addMileage
on ticket
after insert
as
  declare @airnum varchar(20),@date date,
          @start_station varchar(20),@end_station varchar(20),@mileage int,
          @id varchar(20),@comp_name varchar(20) 
  select @airnum = airnum,@date = date,@id = id
  from inserted
  select @start_station = (select start_station from flight where airnum = @airnum and date = @date)
  select @end_station = (select end_station from flight where airnum = @airnum and date = @date)
  select @comp_name = (select comp_name from flight where airnum = @airnum and date = @date)
  select @mileage = (select mileage from airline where start_station = @start_station 
                                                   and end_station = @end_station)
  update member
  set mileage = mileage + @mileage
  where id = @id and comp_name = @comp_name  
  
  select @mileage = (select mileage from member where id = @id and comp_name = @comp_name)
  if(@mileage > 40000)
  begin
  update member
  set mlevel = '银卡会员'
  where id = @id and comp_name = @comp_name 
  end
  else if(@mileage > 80000)
   begin
  update member
  set mlevel = '金卡会员'
  where id = @id and comp_name = @comp_name 
  end
go
--测试
select * from member

--触发器2,退票成功后为乘客减少对应的里程数
if exists(select * from sysobjects where name = 'deleteMileage' and type = 'tr')
drop trigger deleteMileage
go
create trigger deleteMileage
on ticket
after update
as
  declare @airnum varchar(20),@date date,
          @start_station varchar(20),@end_station varchar(20),@mileage int,
          @id varchar(20),@comp_name varchar(20) 
  select @airnum = airnum,@date = date,@id = id
  from deleted
  select @start_station = (select start_station from flight where airnum = @airnum and date = @date)
  select @end_station = (select end_station from flight where airnum = @airnum and date = @date)
  select @comp_name = (select comp_name from flight where airnum = @airnum and date = @date)
  select @mileage = (select mileage from airline where start_station = @start_station 
                                                   and end_station = @end_station)
  update member
  set mileage = mileage - @mileage
  where id = @id and comp_name = @comp_name

  select @mileage = (select mileage from member where id = @id and comp_name = @comp_name)
  if(@mileage < 40000)
   begin
     update member
     set mlevel = '普通会员'
     where id = @id and comp_name = @comp_name 
   end
  else if(@mileage < 80000)
   begin
    update member
    set mlevel = '银卡会员'
    where id = @id and comp_name = @comp_name 
   end
go
--测试
select * from member

select count(*) from ticket where ticketnum like '0603'+'%'
