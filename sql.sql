create database iot;
use iot;
create table iot.device(
	id int primary key auto_increment,
    date date,
    time float,
    type varchar(50)
);
select * from iot.device;
