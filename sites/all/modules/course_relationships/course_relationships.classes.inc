<?php

/**
 * @file
 * Course object classes for course relationships.
 */

class CourseObjectCourse extends CourseObjectNode {

  /**
   * Valid node types are courses.
   */
  function getNodeTypes() {
    return course_get_types();
  }

  /**
   * Set up defaults.
   */
  function optionsDefinition() {
    $definition = parent::optionsDefinition();
    $definition['course_outline'] = 'course';
    $definition['node_type'] = 'course';
    return $definition;
  }

  function optionsForm(&$form, &$form_state) {
    parent::optionsForm($form, $form_state);

    // Course outline display handler.
    $outlines = array();
    $handlers = course_get_handlers('outline');
    foreach ($handlers as $outline_handlers) {
      if ($outline_handlers) {
        foreach ($outline_handlers as $key => $outline_handler) {
          $outlines[$key] = $outline_handler['name'];
        }
      }
    }

    $form['node']['course_outline'] = array(
      '#title' => t('Course outline display'),
      '#type' => 'select',
      '#options' => $outlines,
    );
  }

  /**
   * Create the new course with our node type and outline selection.
   */
  public function create($node = NULL) {
    $node = new stdClass;
    $node->type = $this->getOption('node_type');
    $node->title = $this->getTitle();
    $node->uid = $this->user->uid;
    $node->course['outline'] = $this->getOption('course_outline');
    node_save($node);
    $this->setNode($node);
  }

}
