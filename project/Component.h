class Component
{
public:
	// the lower the update order, the earlier the component updates
	Component(class Actor* owner, int updateOrder = 100);
	virtual ~Component();

	//Update this component by delta time
	virtual void Update(float deltaTime);
	int GetUpdateOrder() const { return mUpdateOrder; }
protected:
	class Actor* mOwner;
	int mUpdateOrder;
};