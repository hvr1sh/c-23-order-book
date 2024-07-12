#include <iostream>
#include <fstream>
#include <map>
#include <vector>

using namespace std;

struct Order {
    int id;
    double price;
    int quantity;
    int type;
};

class OrderBook {
public:
    void addOrder(const Order& order);
    void matchOrders();

private:
    std::multimap<double, Order, std::greater<double> > buyOrders; // Sorted by price descending
    std::multimap<double, Order, std::less<double> > sellOrders;
    // Sorted by price ascending
};

void OrderBook::addOrder(const Order& order)
{
    if(order.type == 1)//buy
    {
        buyOrders.insert(std::make_pair(order.price, order));
    }
    else{
        sellOrders.insert(std::make_pair(order.price, order));
    }
}

void OrderBook::matchOrders()
{
    while(!buyOrders.empty() && !sellOrders.empty())
    {
        auto buyOrder = buyOrders.begin();
        auto sellOrder = sellOrders.begin();

        std::cout << "Matching Buy Order ID " << buyOrder->second.id
                  << " (Price: " << buyOrder->first << ", Quantity: " << buyOrder->second.quantity << ")"
                  << " with Sell Order ID " << sellOrder->second.id
                  << " (Price: " << sellOrder->first << ", Quantity: " << sellOrder->second.quantity << ")" << std::endl;
        if(buyOrder->first >= sellOrder->first)
        {
            int quantity_matched = min(sellOrder->second.quantity, buyOrder->second.quantity);
            sellOrder->second.quantity-=quantity_matched;
            buyOrder->second.quantity=quantity_matched;
if (sellOrder->second.quantity == 0) {
    sellOrders.erase(sellOrder++);
} else {
    ++sellOrder; // Move to the next sell order
}

if (buyOrder->second.quantity == 0) {
    buyOrders.erase(buyOrder++);
} else {
    ++buyOrder; // Move to the next buy order
}

        }
    
    }

}


int main()
{
    OrderBook orderBook;
    std::ifstream infile("orders.txt");
    if (!infile) {
        std::cerr << "Could not open orders.txt" << std::endl;
        return 1;
    }

    int id, quantity;
    double price;
    int typeStr;
    while (infile >> id >> typeStr >> price >> quantity) {
                Order order = {id, price, quantity, typeStr};
        orderBook.addOrder(order);
    }
orderBook.matchOrders();
exit(0);
}