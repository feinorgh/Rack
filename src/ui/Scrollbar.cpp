#include <ui/Scrollbar.hpp>
#include <ui/ScrollWidget.hpp>
#include <context.hpp>
#include <window.hpp>


namespace rack {
namespace ui {


// Internal not currently used


Scrollbar::Scrollbar() {
	box.size = math::Vec(BND_SCROLLBAR_WIDTH, BND_SCROLLBAR_HEIGHT);
}


Scrollbar::~Scrollbar() {
}


void Scrollbar::draw(const DrawArgs& args) {
	ScrollWidget* sw = dynamic_cast<ScrollWidget*>(parent);
	assert(sw);

	BNDwidgetState state = BND_DEFAULT;
	if (APP->event->getHoveredWidget() == this)
		state = BND_HOVER;
	if (APP->event->getDraggedWidget() == this)
		state = BND_ACTIVE;

	float handleOffset = sw->getHandleOffset()[vertical];
	float handleSize = sw->getHandleSize()[vertical];
	bndScrollBar(args.vg, 0.0, 0.0, box.size.x, box.size.y, state, handleOffset, handleSize);
}


void Scrollbar::onButton(const event::Button& e) {
	if (e.button == GLFW_MOUSE_BUTTON_LEFT && e.action == GLFW_PRESS) {
		ScrollWidget* sw = dynamic_cast<ScrollWidget*>(parent);
		assert(sw);

		float pos = e.pos[vertical];
		pos /= box.size[vertical];
		float handleOffset = sw->getHandleOffset()[vertical];
		float handleSize = sw->getHandleSize()[vertical];
		float handlePos = math::rescale(handleOffset, 0.f, 1.f, handleSize / 2.f, 1.f - handleSize / 2.f);

		// Check if user clicked on handle
		if (std::fabs(pos - handlePos) > handleSize / 2.f) {
			// Jump to absolute position of the handle
			float offset = math::rescale(pos, handleSize / 2.f, 1.f - handleSize / 2.f, 0.f, 1.f);
			sw->offset[vertical] = sw->containerBox.pos[vertical] + offset * (sw->containerBox.size[vertical] - sw->box.size[vertical]);
		}
	}
	OpaqueWidget::onButton(e);
}


void Scrollbar::onDragStart(const event::DragStart& e) {
}


void Scrollbar::onDragEnd(const event::DragEnd& e) {
}


void Scrollbar::onDragMove(const event::DragMove& e) {
	ScrollWidget* sw = dynamic_cast<ScrollWidget*>(parent);
	assert(sw);

	// Move handle absolutely.
	float mouseDelta = e.mouseDelta[vertical];
	mouseDelta /= getAbsoluteZoom();

	float handleSize = sw->getHandleSize()[vertical];
	float handleBound = (1.f - handleSize) * box.size[vertical];
	float offsetBound = sw->getContainerOffsetBound().size[vertical];
	float offsetDelta = mouseDelta * offsetBound / handleBound;
	sw->offset[vertical] += offsetDelta;
}


} // namespace ui
} // namespace rack