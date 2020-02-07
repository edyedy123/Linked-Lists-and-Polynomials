#include <memory>//for swap()

template <typename T>
class SortedList{
	struct Node{
		T data_;
		Node* next_;
		Node* prev_;
		Node(const T& data=T{},Node* nx=nullptr,Node* pr=nullptr){
			data_ = data;
			next_ = nx;
			prev_ = pr;
		}
	};
	Node* front_;
	Node* back_;
	int listSize;
public:
	class const_iterator{
		friend class SortedList;
	protected:
		 Node* curr_;
		 const_iterator(Node* x) { this->curr_ = x; }
	public:
		const_iterator(){ this->curr_ = nullptr; }
		Node* getNode() { return this->curr_; }
		const_iterator operator++(){ this->curr_ = this->curr_->next_;return *this; }
		const_iterator operator++(int){
			const_iterator tmp(*this);
			this->curr_ = this->curr_->next_;
			return tmp;
		}
		const_iterator operator--(){ this->curr_ = this->curr_->prev_;return *this; }
		const_iterator operator--(int){
			const_iterator tmp(*this);
			this->curr_ = this->curr_->prev_;
			return tmp;
		}
		bool operator==(const_iterator rhs){ return this->curr_ == rhs.curr_; }
		bool operator!=(const_iterator rhs){ return this->curr_ != rhs.curr_; }
		const T& operator*()const{ return this->curr_->data_; }
	};
	class iterator:public const_iterator{
		friend class SortedList;
	protected:
		iterator(Node* x) { this->curr_ = x; }
	public:
		iterator(){ this->curr_ = nullptr; }
		Node* getNode() { return this->curr_; }
		iterator operator++(){ this->curr_ = this->curr_->next_;return *this; }
		iterator operator++(int){
			iterator tmp(*this);
			this->curr_ = this->curr_->next_;
			return tmp;
		}
		iterator operator--(){ this->curr_ = this->curr_->prev_;return *this; }
		iterator operator--(int){
			iterator tmp(*this);
			this->curr_ = this->curr_->prev_;
			return tmp;
		}
		T& operator*(){ 
			return this->curr_->data_;
		}
		const T& operator*()const{ return this->curr_->data_; }
	};
	//initializer to safe state function
	void init(){
		front_ = new Node();
		back_ = new Node();
		front_->next_ = back_;
		back_->prev_ = front_;
		listSize = 0;
	}
	SortedList();
	~SortedList();
	//copy constructors
	SortedList(const SortedList& rhs);
	SortedList& operator=(const SortedList& rhs);
	//move constructors
	SortedList(SortedList&& rhs);
	SortedList& operator=(SortedList&& rhs);

	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

	iterator insert(const T& data);// inserts data to linked list sorted from smallest to largest
	iterator push_back(const T& data);//inserts data to the back of the linked list(used in insert())
	iterator push_front(const T& data);//inserts data to the front of the linked list(used in insert())
	iterator search(const T& data);
	const_iterator search(const T& data) const;
	iterator erase(iterator it);
	iterator erase(iterator first, iterator last);
	bool empty() const;
	int size() const;
	void swap(SortedList& first, SortedList& second);//uses swap() from #include <memory> to swap ownership of members between two lists
};

template <typename T>
SortedList<T>::SortedList(){
	init();
}
template <typename T>
SortedList<T>::~SortedList(){
	Node* curr = front_;
	while (curr) {
		Node* rm = curr;
		curr = curr->next_;
		delete rm;
	}
}
//copy constructors
template <typename T>
SortedList<T>::SortedList(const SortedList& rhs){
	init();
	listSize = rhs.listSize;
	for (const_iterator it = rhs.begin(); it != rhs.end();++it) {
		push_back(*it);
	}
}
template <typename T>
SortedList<T>& SortedList<T>::operator=(const SortedList& rhs){
	SortedList temp(rhs);
	swap(*this,temp);
	return *this;
}

//move constructors
template <typename T>
SortedList<T>::SortedList(SortedList&& rhs){
	init();
	swap(*this, rhs);
}

template <typename T>
SortedList<T>& SortedList<T>::operator=(SortedList&& rhs){
	SortedList temp(std::move(rhs));
	swap(*this, temp);
	return *this;
}

//Takes in data and inserts it into the list in ascending order, returns itirator pointing at new inseted node.
template <typename T>
typename SortedList<T>::iterator SortedList<T>::insert(const T& data){
	iterator it = begin();
	//if smaller than first node, insert in the front
	if (data < *it || *it == data || listSize == 0) {
		listSize++;
		return push_front(data);
	}
	else {
		//adds new node before current iterator spot if true
		while (it != back_) {
			if (data < *it) {
				Node *curr = it.getNode();
				Node *adding = new Node(data, curr, curr->prev_);

				curr->prev_->next_ = adding;
				curr->prev_ = adding;
				listSize++;

				return iterator(adding);
			}
			it++;
		}
		//if data is bigger than everything in the list, put in the back
		listSize++;
		return push_back(data);
	}

}
//search function takes in data, loops through the list and returns an iterator pointing to the node with matching data if found 
//returns an iterator to the end of the list  
template <typename T>
typename SortedList<T>::iterator SortedList<T>::search(const T& data){
	for (iterator it = begin(); it != end();++it) {
		if (*(it) == data) {
			return it;
		}
	}
	return iterator(end());
}
//const_iterator vesrion of search
template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::search(const T& data) const{
	for (const_iterator it = begin(); it != end();++it) {
		if (*(it) == data) {
			return it;
		}
	}
	return const_iterator(end());
}

//removes node pointed by iterator and points to the next node
template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator it){
		Node *temp = it.getNode();
		temp->next_->prev_ = temp->prev_;
		temp->prev_->next_ = temp->next_;
		it++;
		delete temp;
		listSize--;
		return it;
}

//removes the nodes between the nodes referred to by iterator first to last. This includes the node referred to by first but NOT the node referred to by last.
//returns iterator to last
template <typename T>
typename SortedList<T>::iterator SortedList<T>::erase(iterator first, iterator last){
	while (first != last) {
		first = erase(first);
	}
	return last;
}

//returns true if empty false if not empty
template <typename T>
bool SortedList<T>::empty() const{
	return (listSize <= 0);
}

//returns size of list, sentenal nodes not included
template <typename T>
int SortedList<T>::size() const{
	return listSize;
}

template <typename T>
typename SortedList<T>::iterator SortedList<T>::begin(){
	return iterator(front_->next_);
}
template <typename T>
typename SortedList<T>::iterator SortedList<T>::end() {
	return iterator(back_);
}
template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::begin() const {
	return const_iterator(front_->next_);
}
template <typename T>
typename SortedList<T>::const_iterator SortedList<T>::end() const {
	return const_iterator(back_);
}

//uses swap() from #include <memory> to swap ownership of members between two lists
template <typename T>
void SortedList<T>::swap(SortedList& first, SortedList& second){
	std::swap(first.listSize, second.listSize);
	std::swap(first.front_, second.front_);
	std::swap(first.back_, second.back_);
}

//inserts data to the back of the linked list(used in insert())
template <typename T>
typename SortedList<T>::iterator SortedList<T>::push_back(const T& data) {
	Node *temp = new Node(data, back_, back_->prev_);
	if (front_->next_ == back_) {
		front_->next_ = temp;
	}
	else {
		back_->prev_->next_ = temp;
	}
	back_->prev_ = temp;

	return iterator(temp);
}

//inserts data to the front of the linked list(used in insert())
template <typename T>
typename SortedList<T>::iterator SortedList<T>::push_front(const T& data) {
	Node *temp = new Node(data, front_->next_, front_);
	if (back_->prev_ == front_) {
		back_->prev_ = temp;
	}
	else {
		front_->next_->prev_ = temp;
	}
	front_->next_ = temp;

	return iterator(temp);
}