#include "sched.h"
#include <linux/slab.h>
#include <linux/list.h>

void init_mysched_rq(struct mysched_rq * mysched_rq) {

	INIT_LIST_HEAD(&mysched_rq->queue);

	mysched_rq->nr_running = 0;

	printk(KERN_INFO "***[MYSCHED] Mysched class is online \n");
}

static void update_curr_mysched(struct rq *rq) {}
static void enqueue_task_mysched(struct rq *rq, struct task_struct *p, int flags) {
	struct mysched_rq *mysched_rq = &rq->mysched;
	struct list_head *queue = &mysched_rq->queue;
	struct sched_mysched_entity *mysched_se = &p->mysched;

	//...assignment 5-1
	list_add_tail(&mysched_se->run_list,queue);
	mysched_rq->nr_running++;

	printk(KERN_INFO "***[MYSCHED] Enqueue: nr_running = %u pid = %d\n", mysched_rq->nr_running, p->pid);
}
static void dequeue_task_mysched(struct rq *rq, struct task_struct *p, int flags) {
	//...assignment 5-2
	struct list_head *queue = &rq->mysched.queue;

	if(list_empty(queue)) return;
	
	else{
	list_del_init(&p->mysched.run_list);
	rq->mysched.nr_running--;
	printk(KERN_INFO "***[MYSCHED] Dequeue: nr_running = %u pid = %d\n", rq->mysched.nr_running, p->pid);
	}
}
static void check_preempt_curr_mysched(struct rq *rq, struct task_struct *p, int flags) {}

struct task_struct *pick_next_task_mysched(struct rq *rq, struct task_struct *prev) {
	//...assignment 5-2
	struct mysched_rq *mysched_rq = &rq->mysched;
	struct sched_mysched_entity *mysched_se;
	struct task_struct *p;

	if(!mysched_rq->nr_running)
		return NULL;

	put_prev_task(rq, prev);

	mysched_se = container_of(mysched_rq->queue.next, struct sched_mysched_entity, run_list);
	p = container_of(mysched_se, struct task_struct, mysched); 	
	p->se.exec_start = rq_clock_task(rq);

	return p;
}

static void put_prev_task_mysched(struct rq *rq, struct task_struct *p) {}
static int select_task_rq_mysched(struct task_struct *p, int cpu, int sd_flag, int flags) {
	return task_cpu(p);
}

static void set_curr_task_mysched(struct rq *rq) {}
static void task_tick_mysched(struct rq *rq, struct task_struct *p, int queued) {}
static void prio_changed_mysched(struct rq *rq, struct task_struct *p, int oldprio) {}

/*This routine is called when a task migrates between classes */
static void switched_to_mysched(struct rq *rq, struct task_struct *p) {
	resched_curr(rq);
}

const struct sched_class mysched_sched_class = {
	.next = &fair_sched_class,
	.enqueue_task = enqueue_task_mysched,
	.dequeue_task = dequeue_task_mysched,

	.check_preempt_curr = check_preempt_curr_mysched,

	.pick_next_task = pick_next_task_mysched,
	.put_prev_task = put_prev_task_mysched,

#ifdef CONFIG_SMP
	.select_task_rq = select_task_rq_mysched,
#endif

	.set_curr_task = set_curr_task_mysched,
	.task_tick = task_tick_mysched,
	.prio_changed = prio_changed_mysched,
	.switched_to = switched_to_mysched,

	.update_curr = update_curr_mysched,
};
